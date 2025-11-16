#include <SoftwareSerial.h>

#include <header.hpp>

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  setupPushTelegram();

  // Initialize device.
  // reserveMemory(); // не работает  с такой конфигурацией (вместе с телеграмом)
  dhtBegin();
}

void loop(void)
{
  if (wifiConnected())
  {
    if (mqttConnected())
    {
      mqttLoop();
      readTemperature();
      readVoltage();
    }
  }

  delay(10000);
}
