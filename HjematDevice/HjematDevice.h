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
    byte _data[4];
    byte _id;
  
    byte createHeader(byte command);
    void handleReturnValue(byte dataID);
    void handleDataChange(byte message[]);
    void sendPing();
    
    void tWrite(byte bytes[], int numBytes);
    void tWrite(byte bytes[]);
    
};

#endif
