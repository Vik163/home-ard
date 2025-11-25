#include <SoftwareSerial.h>

#include <header.hpp>

const int THRESHOLD = 190;
int k_stat = 0;
int k_volt = 0;
int k_threshold = 0;

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  wifiConnect();

  initSD();

  setupPushTelegram();

  dhtBegin();

  login();
}

void loop(void)
{
  if (wifiIsConnected())
  {
    if (mqttConnected())
    {
      mqttLoop();

      readTemperature();
      int value = readVoltage();

      if (value < THRESHOLD) // счетчик минимального напряжения
      {
        k_threshold++;
      }

      k_volt++;

      if (k_volt == 1 && value > min_limit && value < max_limit) // каждые 15 сек записывает в файл показания напряжения
      {
        writeFile(fileVolt, String(value).c_str());
        k_volt = 0;
      }

      k_stat++;

      if (k_stat == statistics_interval) // каждые 20 мин = 240 обрабатывает накопленные данные
      {
        setStatisticsData();
        k_stat = 0;
        setTimeThreshold(k_threshold);
        k_threshold = 0;
      }
    }
  }

  delay(5000);
}
