#include "HjematTherometer.h"
#include "HjematDevice.h"

#include "Arduino.h"

#define therometerPin A5

float tempC;
int reading;

short HjematTherometer::returnValue(byte dataID) {
  if (dataID == 1) {
    reading = analogRead(therometerPin);

    tempC = reading * 500.0;
    tempC /= 1024.0;
     
    return round(tempC);
  }

  return 0;
}

short HjematTherometer::dataChange(byte dataID, short value) {
  return 0;
}




