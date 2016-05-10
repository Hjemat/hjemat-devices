#include <HjematDevice.h>
#include "HjematTestLight.h"

HjematTestLight* device = new HjematTestLight();
void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  device->loop();
}
