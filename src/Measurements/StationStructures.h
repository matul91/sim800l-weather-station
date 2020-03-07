#include "Arduino.h"

#ifndef _STATION_STRUCTURES_H_
#define _STATION_STRUCTURES_H_

struct Wind
{  
   float speed; 
   float gust;
   std::string direction; 
};

struct Temperature
{  
   float internal; 
   float external;
};

struct Humidity
{  
   float internal; 
   float external;
};

struct Pressure
{  
   float rawValue;
   float altitude;
   float atSeaLevel;
};

struct WeatherStationStatus
{ 
   Wind wind;
   Temperature temperature;
   Humidity humidity;
   Pressure pressure;
   float voltage; 
   uint8_t signalStrength;
};

#endif