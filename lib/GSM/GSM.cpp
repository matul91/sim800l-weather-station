#include "GSM.h"
#include "Arduino.h"

void GSM::init() 
{
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(100);

  #ifdef DUMP_AT_COMMANDS
    #include <StreamDebugger.h>
    SerialMon.begin(115200);
    StreamDebugger debugger(SerialAT, SerialMon);
    TinyGsm modem(debugger);
  #else
    TinyGsm modem(SerialAT);
  #endif

  TinyGsmClientSecure client(modem);
  sim800l = new SIM800L((Stream *)&SerialAT, MODEM_RST, 200, 1024, (Stream *)&SerialMon);

  modem.init();  
}

void GSM::connectToInternet() 
{
  bool connected = false;
  for(uint8_t i = 0; i < 15 && !connected; i++) {
    delay(2000);
    connected = sim800l->connectGPRS();
  }
}

void GSM::sendPayload(String payload) 
{
  char postPayload[payload.length()]; 
  strcpy(postPayload, payload.c_str());
  sim800l->doPost(URL, CONTENT_TYPE, postPayload, 10000, 10000);
}

uint8_t GSgetSignalStrenght() 
{
  return sim800l->getSignal();
}