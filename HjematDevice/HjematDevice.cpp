#include "Arduino.h"
#include "HjematDevice.h"

// Defination of pins and commands numbers
#define rwTogglePin 2
#define buttonPin A1
#define debugPin 13

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
  // Converts the product id from a long value (32 bit number) to byte array
  // The byte array is easier to work with later
  this->_productID[0] = productID >> 16;
  this->_productID[1] = productID >> 8;
  this->_productID[2] = productID;

  // Setting up Pins
  pinMode(rwTogglePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  // Making sure the rw pin is low, so we can read
  digitalWrite(rwTogglePin, LOW);
}

// Executed at as long as program runs
void HjematDevice::loop() {
  // We don't have an ID, pairing is allowed and we have no send a pair request...
  if (_id == 0 && _pairingAllowed && !_pairRequestSend) {
    // ... so when user has pressed the button, we send the pair request
    if (digitalRead(buttonPin) == HIGH) {
      sendPairRequest();
      _pairRequestSend = true;
    }
  }

  // Stop execution if there's no data to read
  if (Serial.available() == 0)
    return;

  // Empty the message array
  _message[0] = 0;
  _message[1] = 0;
  _message[2] = 0;
  _message[3] = 0;

  // Read 4 bytes to the message array
  Serial.readBytes(_message, 4);

  if (_message[0] != 0) {           // We got valid message
    // Gets message id from message: XXXXXYYY >> 3 -> 000XXXXX
    _messageID = (_message[0] >> 3);
    // Gets message command: XXXXXYYY & OOOOO111 -> 00000YYY
    _messageCommand = _message[0] & B00000111;
    
    // The message is a pair message, and we don't have an id, so it's relevant
    if (_messageID == 0 && _id == 0)
    {      
      switch(_messageCommand) {
        case(pairComAllow):
          // We can now send pair requests
          _pairingAllowed = true;
          break;
        case(pairComReturn):
          // If we have send a request for id, we assign the containing id for ourself
          // And send a ping back to confirm that we got the id
          if (_pairRequestSend) {
            _id = _message[1];
            _pairingAllowed = false;
            sendPing();
          }
          break;
        case(pairComStop):
          // We must not send requests anymore so no pairing allowed
          _pairingAllowed = false;
          _pairRequestSend = false;
          break;
        default:
          break;
      }
    } // The message is appointed to our id
    else if (_messageID == _id) {
      // We call methods based on the message's command
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

// Send a pair request
void HjematDevice::sendPairRequest()
{
  // Creates the message header with id 0 and the pair requets command
  byte header = createHeader(0, pairComRequest);

  // Create a byte array with header as first byte and the product id filling the rest
  // This the structure of a ping back message which we can send
  byte returnMessage[4] = { header, _productID[0], _productID[1], _productID[2] };

  // Sending the message
  tWrite(returnMessage);
}

// Sends ping back
void HjematDevice::sendPing()
{
  // Creates header for message with the device's id and the BACK command
  byte header = createHeader(comBack);

  // The message itself with header and product id
  byte pingback[4] = { header, _productID[0], _productID[1], _productID[2] };

  // Sends the message
  tWrite(pingback);
}

// Creates a header with id and command
byte HjematDevice::createHeader(byte id, byte command)
{
  // Shifts id from 000XXXXX to XXXXX000
  byte header = id << 3;
  // Adds command with OR: XXXXX000 | 00000YYY -> XXXXXYYY
  header = header | command;
  return header;
}

// Creates a message with the device's id
byte HjematDevice::createHeader(byte command)
{
  return createHeader(_id, command);
}

// Handles when the device is requested for data
void HjematDevice::handleReturnValue(byte dataID)
{
  short value;
  // Calls the device specific returnValue method
  // This should be implemented in devices inheriting this class
  value = returnValue(dataID);
  
  // Create message with header with device id and command RETURN
  // data id of the requested data and the data in the last to bytes
  byte header = createHeader(comReturn);
  byte returnMessage[4] = { header, dataID, highByte(value), lowByte(value) };

  // Sends the message to return data
  tWrite(returnMessage);
}

// Handles when the devices is requested to change data
void HjematDevice::handleDataChange(byte message[])
{
  // Gets the data id from the message
  byte dataID =  message[1];

  // Gets the value to change to, from the message
  // This converts to bytes to a 16-bit number
  short value = message[2];
  value = value << 8;
  value = value | message[3];

  // Calls the device specific dataChange method, implemented for the specific device
  value = dataChange(dataID, value);

  // Uses handle return value to confirm the data change
  handleReturnValue(dataID);
}

// A function for reading to RS485
void HjematDevice::tWrite(byte bytes[], int numBytes)
{
  // Set the rw pin to HIGH so we can send data to RS485
  digitalWrite(rwTogglePin, HIGH);
  delay(5);

  // Writes the bytes to serial
  Serial.write(bytes, numBytes);

  delay(5);

  // Set the rw pin to LOW so we can receive messages again
  digitalWrite(rwTogglePin, LOW);
}

void HjematDevice::tWrite(byte bytes[])
{
  tWrite(bytes, 4);
}


