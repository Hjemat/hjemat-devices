#include <HjematDevice.h>
#include "HjematReed.h"

HjematReed* device = new HjematReed();
void setup() {
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  device->loop();
}
