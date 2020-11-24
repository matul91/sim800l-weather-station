#include "Arduino.h"
#include "VentusDecoder.cpp"
#include "AuriolDecoder.cpp"
#include <map>
#include "PowerManagement.h"

#define RECEIVER_DATA_PIN_NUMBER 13
#define RECEIVER_POWER_PIN_NUMBER 12
#define RECEIVER_CS_PIN_NUMBER 14

struct Windy
{  
   float speed; 
   float gust;
   std::string direction; 
};

class VentusReceiver
{
protected:
  std::map<int, std::string> directions = {
      { 0, "N" },
      { 45, "NE" },
      { 90, "E" },
      { 135, "SE" },
      { 180, "E" },
      { 225, "SW" },
      { 270, "W" },
      { 315, "NW" }, 
  };
    
public:
  gpio_num_t dataPin = (gpio_num_t)(RECEIVER_DATA_PIN_NUMBER & 0x1F);
  
  struct Windy wind = {};
  float temperature = 0;
  float humidity = 0;

  boolean speedOK = false;
  boolean temperatureAndHumidityOK = false;
  boolean gustAndDirectionOK = false;
   
  void enable()
  {
    pinMode(RECEIVER_DATA_PIN_NUMBER, INPUT);
    
    pinMode(RECEIVER_POWER_PIN_NUMBER, OUTPUT);
    pinMode(RECEIVER_CS_PIN_NUMBER, OUTPUT);
    digitalWrite(RECEIVER_POWER_PIN_NUMBER, HIGH);
    digitalWrite(RECEIVER_CS_PIN_NUMBER, HIGH);
  }

  void disable()
  {
    pinMode(RECEIVER_CS_PIN_NUMBER, OUTPUT);
    pinMode(RECEIVER_POWER_PIN_NUMBER, OUTPUT);
    digitalWrite(RECEIVER_POWER_PIN_NUMBER, LOW);
    digitalWrite(RECEIVER_CS_PIN_NUMBER, LOW);
  }

  void receive()
  {
    VentusDecoder * ventus = new VentusDecoder(); 
    PowerManagement powerManagement;
  
    int state = 0;
    word last = 0;
    word pulse = 0;
    word actualState = 0;

    long time = millis();
    
    while (true)
    {
      actualState = ((GPIO_REG_READ(GPIO_IN_REG) >> dataPin) & 1U);

      if ( actualState != state )
      {
        state = !state;
        pulse = micros() - last;
        last += pulse;
        word p = pulse;

        if (p != 0)
        {

          if (isDecodingFinished() || (millis() - time > 400000)) {
              delete ventus;
              break;
          }
          
          if (ventus->nextPulse(p))
          {
            const std::vector<char> data = ventus->getData();
            
            if (ventus->checkSum()) {
              char result[data.size()];
		          std::copy(data.begin(), data.end(), result);
              decodeWindData(result);
                
              powerManagement.planWakeUpAfter(25 * 1000000ULL);
              powerManagement.startLightSleep();      
            }

            ventus->resetDecoder();
          }
        }
      }
    }
  }

  boolean isDecodingFinished() {
    return speedOK && gustAndDirectionOK && temperatureAndHumidityOK;
  }

  void decodeWindData(const char * encodedBits)
  {
      decodeSpeed(encodedBits);
      decodeDirectionAndGust(encodedBits);
      decodeTemperatureAndHumidity(encodedBits);
  }

  void decodeSpeed(const char * bits) {
    if (bits[9] && bits[10] && bits[12] && !bits[13] && !bits[14] && !bits[15] && !bits[16] && !bits[17] && !bits[18] && !bits[19] && !bits[20] && !bits[21] && !bits[22] && !bits[23])
    {
      unsigned int windAverageSpeed = 0;

      for (int i = 24; i < 32; i++)
      {
        windAverageSpeed |= bits[i] << (i - 24);
      }

      wind.speed =  windAverageSpeed * 0.2F;
      speedOK = true;
    }
  }

  void decodeDirectionAndGust(const char * bits)
  {
    if (bits[9] && bits[10] && bits[12] && bits[13] && bits[14])
    {
      unsigned int direction = 0;
      unsigned int windGust = 0;
      
      for (int i = 15; i < 24; i++)
      {
        direction |= bits[i] << (i - 15);
      }
      
      for (int i = 24; i < 32; i++)
      {
        windGust |= bits[i] << (i - 24);
      }

      
      wind.direction = directions[direction];
      wind.gust = windGust * 0.2F;

      gustAndDirectionOK = true;
    }   
  }

  void decodeTemperatureAndHumidity(const char * bits) {
    if (!bits[9] || !bits[10])
    {
      int decodedTemperature = 0;
    
      for (int i = 12; i < 23; i++)
      {
        decodedTemperature |= bits[i] << (i - 12);
      }
      
      if (bits[23])
      {
        decodedTemperature = -2048 + decodedTemperature;
      }

      temperature = (float) decodedTemperature / 10;

      unsigned int humidityOnes = 0;
      for (int i = 24; i < 28; i++)
      {
        humidityOnes |= bits[i] << (i - 24);
      }

      unsigned int humidityTens = 0;
      for (int i = 28; i < 32; i++)
      {
        humidityTens |= bits[i] << (i - 28);
      }
      
      humidity = humidityTens * 10 + humidityOnes;

      temperatureAndHumidityOK = true;
    }
  }
};
