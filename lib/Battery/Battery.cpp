#include "Battery.h"
#include "Arduino.h"

int Battery::getRemainingCapacity() {
  return remainingCapacity;
}

float Battery::getVoltage() {
  return voltage;
}

void Battery::measure() {
  
  voltage = 0;
  remainingCapacity = 0;
  
  int actualSampleCount = 0;
  float voltageSum = 0.0f;
  
  while (actualSampleCount < MEASURED_SAMPLE_COUNT) {
    voltageSum += analogRead(VOLTAGE_PIN_NUMBER);
    actualSampleCount++;
    delay(DELAY_BETWEEN_MEASUREMENT);
  }

  voltage = ( (voltageSum / MEASURED_SAMPLE_COUNT) / 2350.0 ) * 4.20;
  remainingCapacity = _min(
    map(voltageSum / MEASURED_SAMPLE_COUNT, 2000, 2440.0, 0, 100),
    100
  ); 
}