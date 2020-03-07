#include "PowerManagement.h"
#include "GSM.h"
#include "Measurements/Measurements.h"
#include "PayloadFactory/PayloadFactory.h"

void setup() {
   Serial.begin(115200);
}

void loop() {
  unsigned long wakeupTime = micros();

  PowerManagement powerManagement;
  powerManagement.init();
  powerManagement.enablePowerBoost();
  
  Measurements measurements;
  WeatherStationStatus status = measurements.getAll();

  GSM gsm;
  gsm.init();
  gsm.connectToInternet();
  status.signalStrength = gsm.getSignalStrenght();

  PayloadFactory PayloadFactory;
  String payload = PayloadFactory.createFrom(status);
  gsm.sendPayload(payload);

  uint64_t uptime = (micros() - wakeupTime) * 1ULL;
  uint64_t sleepTime = (ONE_HOUR * 1000000ULL) - uptime;

  powerManagement.planWakeUpAfter(sleepTime);
  powerManagement.startDeepSleep();
}