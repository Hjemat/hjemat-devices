#include "HjematTherometer.h"
#include "HjematDevice.h"

#include "Arduino.h"

#define therometerPin A1

short HjematTherometer::returnValue(byte dataID) {
  if (dataID == 1) {
    return analogRead(therometerPin);
  }

  return 0;
}

short HjematTherometer::dataChange(byte dataID, short value) {
  return 0;
}




