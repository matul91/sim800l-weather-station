#include <Wire.h>

#define IP5306_ADDR                 0x75
#define IP5306_REG_SYS_CTL0         0x00

#define I2C_SDA                     21
#define I2C_SCL                     22

#define uS_TO_S_CONVERSION_FACTOR   1000000ULL 
#define ONE_HOUR                    3600
#define THIRTY_MINUTES              1800

#ifndef _POWER_MANAGEMENT_H_
#define _POWER_MANAGEMENT_H_

class PowerManagement
{
    public:
        void init(void);
        bool enablePowerBoost(void);
        bool disablePowerBoost(void);
        void planWakeUpAfter(uint64_t timeInMicroSeconds);
        void startDeepSleep();
        void startLightSleep();
    private:
        bool setPowerBoost(uint8_t enabled);
};

#endif