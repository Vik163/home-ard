#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <header.hpp>

const char mqttpTopicTemperature[] = "home/state/temperature";
const char mqttpTopicHumidity[] = "home/state/humidity";
const bool mqttpSensorRetained = false;

#define DHTPIN D1 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// #define DHTTYPE    DHT11     // DHT 22 (AM2302)
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

void readTemperature()
{

   sensors_event_t event;
   dht.temperature().getEvent(&event);
   float t = event.temperature;
   readItem("Temperature: ", t, "*C", mqttpTopicTemperature);

   dht.humidity().getEvent(&event);
   float h = event.relative_humidity;
   readItem("Humidity: ", h, "%", mqttpTopicHumidity);
}

void dhtBegin()
{
   dht.begin();
}