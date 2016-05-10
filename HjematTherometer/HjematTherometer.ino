#include <HjematDevice.h>
#include "HjematTherometer.h"

HjematTherometer* device = new HjematTherometer();
void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  device->loop();
}
