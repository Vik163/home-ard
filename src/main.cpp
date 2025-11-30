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
  serverBegin();

  // setupPushTelegram();

  dhtBegin();

  // login();
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
        setVoltValues(value, k_stat);
        k_stat++;
        k_volt = 0;
      }

      if (k_stat == 3) // каждые 20 мин = 240 обрабатывает накопленные данные
      {
        setStatisticsData(k_threshold);
        k_stat = 0;
        k_threshold = 0;
      }
    }
  }

  delay(5000);
}
