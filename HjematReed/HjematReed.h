#ifndef HJEMATREED_H
#define HJEMATREED_H

#include "HjematDevice.h"
#include "Arduino.h"

class HjematReed : public HjematDevice {
  public:
    HjematReed() : HjematDevice(3) { 
      pinMode(12, INPUT);
    }
    short returnValue(byte dataID);
    short dataChange(byte dataID, short value);
};

#endif

