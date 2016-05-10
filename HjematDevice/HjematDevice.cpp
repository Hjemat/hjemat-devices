#include "Arduino.h"
#include "HjematDevice.h"

#define rwTogglePin 2
#define buttonPin A0

#define comPing     0x1
#define comBack     0x2
#define comGet      0x3
#define comSet      0x4
#define comReturn   0x5

#define pairComAllow   0x1
#define pairComRequest 0x2
#define pairComReturn  0x3
#define pairComStop    0x4

HjematDevice::HjematDevice(long productID)
{
  this->_productID[0] = productID >> 16;
  this->_productID[1] = productID >> 8;
  this->_productID[2] = productID;

  pinMode(rwTogglePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  digitalWrite(rwTogglePin, LOW);
}

void HjematDevice::loop() {
  if (Serial.available() == 0)
    return;
    
  Serial.readBytes(_message, 4);

  if (_id == 0 && _pairingAllowed) {
    if (digitalRead(buttonPin) == HIGH) {
      sendPairRequest();
      _pairRequestSend = false;
    }
  }

  if (_message[0] != 0) {           // We got a message
    _messageID = _message[0] >> 3;
    _messageCommand = _message[0] & B00000111;
    
    if (_messageID == _id){      //The message is refering to this device's id
      if (_messageID == 0 ) {           // The message is about pairing
        switch(_messageCommand) {
          case(pairComAllow):
            _pairingAllowed = true;
            break;
          case(pairComReturn):
            if (_pairRequestSend) {
              _id = _message[1];
            }
            break;
          case(pairComStop):
            _pairingAllowed = false;
            _pairRequestSend = false;
            break;
          default:
            break;
        }
      }
      else {                            // The message is a command
        switch(_messageCommand) {
          case(comPing):
            sendPing();
            break;
          case(comGet):
            handleReturnValue(_message[1]);
            break;
          case(comSet):
            handleDataChange(_message);
            break;
          default:
            break;
        }
      }
    }
  }
}

void HjematDevice::sendPairRequest()
{
  byte header = createHeader(0, pairComRequest);
  byte returnMessage[4] = { header, _productID[0], _productID[1], _productID[2] };

  tWrite(returnMessage);
}

void HjematDevice::sendPing()
{
  byte header = createHeader(comBack);
  byte pingback[4] = { header, this->_productID[0], this->_productID[1], this->_productID[2] };
  delay(100);
  tWrite(pingback, 4);
}

byte HjematDevice::createHeader(byte command)
{
  return createHeader(_id, command);
}

byte HjematDevice::createHeader(byte id, byte command)
{
  byte header = id << 3;
  header = header | command;
  return header;
}

void HjematDevice::handleReturnValue(byte dataID)
{
  short value;
  
  value = returnValue(dataID);
  
  byte header = createHeader(comReturn);
  byte returnMessage[4] = { header, dataID, (byte)(value >> 8), (byte)value };

  tWrite(returnMessage, 4);
}

void HjematDevice::handleDataChange(byte message[])
{
  byte dataType =  message[1];
  short value = message[2];
  
  value = value << 8;
  value = value | message[3];

  value = dataChange(dataType, value);

  handleReturnValue(dataType);
}

void HjematDevice::tWrite(byte bytes[], int numBytes)
{
  //Serial.println("Writing fine data");
  PORTB = PORTB | B00100000;
  digitalWrite(rwTogglePin, HIGH);
  delay(5);
  Serial.write(bytes, numBytes);

  delay(5);
  digitalWrite(rwTogglePin, LOW);
}

void HjematDevice::tWrite(byte bytes[])
{
  tWrite(bytes, 4);
}


