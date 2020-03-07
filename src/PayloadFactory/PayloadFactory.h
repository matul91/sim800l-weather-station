#include "Arduino.h"
#include "Measurements/StationStructures.h"

#ifndef _PAYLOAD_FACTORY_H_
#define _PAYLOAD_FACTORY_H_

class PayloadFactory
{
    public:
        String createFrom(WeatherStationStatus status);

};

#endif