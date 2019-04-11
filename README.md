# SHT31-Curie
Drivers for Sensirion SHT31 - Arduino 101 with Intel Curie

Example:

#include "RHTsensorSHT31.h"
RHTsensorSHT31 sensor = RHTsensorSHT31();

void setup() {
  Serial.begin(115200);
}

void loop() {
  sensor.updateRHT();
}
