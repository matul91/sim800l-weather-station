#include "Seeed_BME280.h"

#define BMP_280_POWER_PIN_NUMBER       4
#define WEATHER_STATION_ALTITUDE       210.4

#ifndef _PRESSURE_SENSOR_H_
#define _PRESSURE_SENSOR_H_

class PressureSensor
{
    public:
        BME280 bme280;
        void enable(void);
        void disable(void);
        float getPressure(void);
        float getPressureAtSeaLevel(void);  
        float getTemperature(void);  
        float getHumidity(void);  
};

#endif