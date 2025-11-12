#include <SoftwareSerial.h>
#include <header.hpp>

void setup(void)
{
  Serial.begin(9600);
  Serial.println();

  // Initialize device.
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

  delay(5000);
}
