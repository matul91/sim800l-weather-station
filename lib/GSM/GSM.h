#include "Arduino.h"
#include "SIM800L.h"

#ifndef _GSM_H_
#define _GSM_H_


// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// TTGO T-Call pin definitions
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
  
const char APN[] = "internet.t-mobile.cz";
const char URL[] = "https://raspi.jiri-matula.cz/graphql";
const char CONTENT_TYPE[] = "application/json";

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024  

#include <TinyGsmClient.h>

// Uncomment to dump AT commands to Serial
// #define DUMP_AT_COMMANDS

class GSM
{
    public:
        void init(void);
        void disable(void);
        void connectToInternet(void);
        void sendPayload(String payload);
        uint8_t getSignalStrenght(void);
    private:
        SIM800L* sim800l;
};
#endif