#ifndef HJEMATDEVICE_H
#define HJEMATDEVICE_H

#include "Arduino.h"

class HjematDevice
{
  public:
    HjematDevice(long productID);
    void begin(int productID);
    void loop();
    virtual short returnValue(byte dataID) = 0;
    virtual short dataChange(byte dataID, short value) = 0;
  private:
    // Variables
    byte _productID[3];
    byte _id = 0;

    byte _message[4];
    byte _messageID;
    byte _messageCommand;

    bool _pairingAllowed = false;  
    bool _pairRequestSend = false;  
  
    byte createHeader(byte command);
    byte createHeader(byte id, byte command);
    void handleReturnValue(byte dataID);
    void handleDataChange(byte message[]);
    void sendPing();
    void sendPairRequest();
    
    void tWrite(byte bytes[], int numBytes);
    void tWrite(byte bytes[]);
    
};

#endif
