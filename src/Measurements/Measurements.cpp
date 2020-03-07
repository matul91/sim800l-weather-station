#include "SIM800L.h"
#include "Battery.h"
#include "PressureSensor.h"
#include "VentusReceiver.cpp"
#include "Measurements.h"

WeatherStationStatus Measurements::recordVoltage(WeatherStationStatus status) {
  Battery battery;

  battery.measure();
  status.voltage = battery.getVoltage();

  return status;  
}

WeatherStationStatus Measurements::recordFromPressureSensor(WeatherStationStatus status) {
  PressureSensor pressureSensor;  
  
  pressureSensor.enable();
  
  status.pressure.atSeaLevel = pressureSensor.getPressureAtSeaLevel();
  status.pressure.rawValue = pressureSensor.getPressure();
  status.temperature.internal = pressureSensor.getTemperature();
  status.humidity.internal = pressureSensor.getHumidity();
 
  pressureSensor.disable();
  
  return status;
}

WeatherStationStatus Measurements::recordFromWindSensor(WeatherStationStatus status) {
  VentusReceiver ventus;
  ventus.enable();
  
  ventus.receive();

  status.temperature.external = ventus.temperature;
  status.humidity.external = ventus.humidity; 
  status.wind.speed = ventus.wind.speed;
  status.wind.gust = ventus.wind.gust;
  status.wind.direction = ventus.wind.direction;

  ventus.disable();

  return status;
}

WeatherStationStatus Measurements::getAll() { 
  struct WeatherStationStatus status = {};
  
  status = Measurements::recordVoltage(status);
  status = Measurements::recordFromPressureSensor(status);
 
  status = Measurements::recordFromWindSensor(status);

  return status;
}
