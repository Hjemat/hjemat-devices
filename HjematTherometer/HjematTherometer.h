#ifndef HJEMATTHEROMETER_H
#define HJEMATTHEROMETER_H

#include "HjematDevice.h"
#include "Arduino.h"

class HjematTherometer : public HjematDevice {
  public:
    HjematTherometer() : HjematDevice(2) { }
    virtual short returnValue(byte dataID);
    virtual short dataChange(byte dataID, short value);
};

#endif

