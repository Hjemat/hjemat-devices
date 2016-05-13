#include "HjematReed.h"
#include "HjematDevice.h"

#include "Arduino.h"

#define reedPin 12

short HjematReed::returnValue(byte dataID) {
  if (dataID == 1) {
    return digitalRead(reedPin);
  }

  return 0;
}

short HjematReed::dataChange(byte dataID, short value) {
  return 0;
}




