#include "HjematTestLight.h"
#include "HjematDevice.h"

#include "Arduino.h"

#define therometerPin A1

short HjematTestLight::returnValue(byte dataID) {
  if (dataID == 1) {
    return lightValue;
  }

  return 0;
}

short HjematTestLight::dataChange(byte dataID, short value) {
  if (lightValue == 0) {
    lightValue = 1;
    digitalWrite(ledPin, HIGH);
  }
  else {
    lightValue = 0;
    digitalWrite(ledPin, LOW);
  }
}




