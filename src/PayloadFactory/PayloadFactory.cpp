#include "PayloadFactory.h"
#include "Arduino.h"
#include "ArduinoJson.h"

String PayloadFactory::createFrom(WeatherStationStatus status) 
{
const char *query = R""""(
        mutation saveWeatherData(
                $speed: Float, 
                $direction: WindDirection,
                $temperature: Float, 
                $humidity: Float,
                $pressure: Float,
                $voltage: Float,
                $signalStrength: Int
        ) {
                recordWind(speed: $speed, direction: $direction) { speed, direction },
                recordTemperature(temperature: $temperature) { temperature },
                recordHumidity(humidity: $humidity) { humidity },
                recordPressure(pressure: $pressure) { pressure },
                recordBatteryStatus(voltage: $voltage) { voltage },
                recordNetworkStatus(signal_strength: $signalStrength ) { signal_strength }
        }
)"""";

  DynamicJsonDocument json(1024);

  json["query"] = query;
  json["operationName"] = "saveWeatherData";

  JsonObject variables = json.createNestedObject("variables");
  variables["speed"] = status.wind.gust;
  variables["direction"] = status.wind.direction;
  variables["temperature"] = status.temperature.external;
  variables["humidity"] = status.humidity.external;
  variables["pressure"] = (int) status.pressure.atSeaLevel;
  variables["signalStrength"] = status.signalStrength;
  variables["voltage"] = status.voltage;

  return json.as<String>();
};