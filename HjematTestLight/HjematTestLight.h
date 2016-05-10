#ifndef HJEMATTESTLIGHT_H
#define HJEMATTESTLIGHT_H

#include "HjematDevice.h"
#include "Arduino.h"

#define ledPin 6

class HjematTestLight : public HjematDevice {
  public:
    HjematTestLight() : HjematDevice(1) {
      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);
    }
    virtual short returnValue(byte dataID);
    virtual short dataChange(byte dataID, short value);
  private:
    int lightValue = 0;
};

#endif

