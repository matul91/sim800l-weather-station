#include "PressureSensor.h"
#include "Arduino.h"

void PressureSensor::enable() {
  pinMode(BMP_280_POWER_PIN_NUMBER, OUTPUT);
  digitalWrite(BMP_280_POWER_PIN_NUMBER, HIGH);
  delay(100);

  bme280.init();
  delay(100);
}

void PressureSensor::disable() {
  pinMode(BMP_280_POWER_PIN_NUMBER, OUTPUT);
  digitalWrite(BMP_280_POWER_PIN_NUMBER, LOW);
}

float PressureSensor::getPressure() {
  return bme280.getPressure() * 0.01;
}

float PressureSensor::getPressureAtSeaLevel() {
  return bme280.getPressure() * 0.01 / pow(1.0 - ((float) WEATHER_STATION_ALTITUDE / 44330.0), 5.255);
}

float PressureSensor::getTemperature() {
  return bme280.getTemperature();
}

float PressureSensor::getHumidity() {
  return bme280.getHumidity();
}

