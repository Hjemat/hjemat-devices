#include <HjematDevice.h>
#include "HjematTherometer.h"

HjematTherometer* device = new HjematTherometer();
void setup() {
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  device->loop();
}
