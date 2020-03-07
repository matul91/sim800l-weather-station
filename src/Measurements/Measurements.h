
#include "Arduino.h"
#include "StationStructures.h"

#ifndef _MEASUREMENTS_H_
#define _MEASUREMENTS_H_

class Measurements
{
    public:
        WeatherStationStatus recordFromPressureSensor(WeatherStationStatus status);
        WeatherStationStatus recordVoltage(WeatherStationStatus status);
        WeatherStationStatus recordFromWindSensor(WeatherStationStatus status); 
        WeatherStationStatus getAll();
};

#endif