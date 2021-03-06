#ifndef HJEMATTHEROMETER_H
#define HJEMATTHEROMETER_H

#include "HjematDevice.h"
#include "Arduino.h"

class HjematTherometer : public HjematDevice {
  public:
    HjematTherometer() : HjematDevice(2) { }
    short returnValue(byte dataID);
    short dataChange(byte dataID, short value);
};

#endif

