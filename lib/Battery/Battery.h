#ifndef _BATTERY_H_
#define _BATTERY_H_

#define MEASURED_SAMPLE_COUNT       10
#define VOLTAGE_PIN_NUMBER          35
#define DELAY_BETWEEN_MEASUREMENT   20

class Battery
{
    public:
        void measure(void);
        int getRemainingCapacity(void);
        float getVoltage(void);  

    private:
        float voltage = 0.0f;
        int remainingCapacity = 0;
};

#endif