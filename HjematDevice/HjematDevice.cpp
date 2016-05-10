#include "Arduino.h"
#include "HjematDevice.h"

#define rwTogglePin 2
#define buttonPin A0

#define comPing     0x1
#define comBack     0x2
#define comGet      0x3
#define comSet      0x4
#define comReturn   0x5

byte _message[4];
byte _messageID;
byte _messageCommand;
byte _id = 0x0;

//byte _handshakeMessage[4] = { B00000101, 0x0, 0x0, 0x10 };

byte _productID[] = { 0x0, 0x0, 0x1 };

HjematDevice::HjematDevice(long productID) {
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

  if (_message[0] != 0) {           // We got a message
    _messageID = _message[0] >> 3;
    _messageCommand = _message[0] & B00000111;
    
    if (_messageID == _id){      //The message is refering to this device's id
      if (_messageID == 0 ) {           // The message is about pairing
        // Handle Pairing
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
        }
      }
    }
  }
/*
  if (_data[0] != 0){
    if (_id == 0x0) {
      if (_data[0] == B00000011) {
        tWrite(handshakeMessage, 4);
      }
      else if (data[0] == B00000101) {
        id = data[1];
      }
    }
    else {
      if (data[0] >> 3 == id){
        //Serial.println("I've been summoned!");
        byte kom = data[0] & B00000111;
        
        if (kom == comSet)
        {
          //Serial.println("I've been told to do something");
          byte dataType =  data[1];
          int value = data[2];
          value = value << 8;
          value = value | data[3];

          handleDataChange(dataType, value);

          handleReturnValue(dataType);
        }
        else if (kom == comGet)
        {
          //Serial.println("I've to tell something");
          byte dataID =  data[1];

          handleReturnValue(dataID);
        }
        else if (kom == comPing)
        {
          //Serial.println("I've have to tell that I'm awake!");
          byte header = createHeader(comBack);
          byte pingback[4] = { header, productID[0], productID[1], productID[2] };
          delay(150);
          tWrite(pingback, 4);
        }
      }
    }
  }*/
}

void HjematDevice::sendPing() {
  byte header = createHeader(comBack);
  byte pingback[4] = { header, this->_productID[0], this->_productID[1], this->_productID[2] };
  delay(100);
  tWrite(pingback, 4);
}

byte HjematDevice::createHeader(byte command)
{
  byte header = this->_id << 3;
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

void HjematDevice::handleDataChange(byte message[]) {
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


