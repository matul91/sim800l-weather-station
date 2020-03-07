#include "PowerManagement.h"

void PowerManagement::init() 
{
     Wire.begin(I2C_SDA, I2C_SCL);
}

bool PowerManagement::enablePowerBoost(void)
{
    return setPowerBoost(0x37);
}

bool PowerManagement::disablePowerBoost(void)
{
    return setPowerBoost(0x35);
}

void PowerManagement::planWakeUpAfter(uint64_t timeInMicroSeconds) 
{  
  esp_sleep_enable_timer_wakeup(timeInMicroSeconds);
}

void PowerManagement::startDeepSleep() 
{  
  esp_deep_sleep_start();
}

void PowerManagement::startLightSleep() 
{  
  esp_light_sleep_start();
}

bool PowerManagement::setPowerBoost(uint8_t enabled) 
{
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    Wire.write(enabled); 

    bool result = Wire.endTransmission() == 0;
    
    return result;
}