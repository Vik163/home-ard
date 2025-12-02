#include <SoftwareSerial.h>

#include <header.hpp>

int k_stat = 0;
int k_stat_5 = 0;
int k_volt = 0;
int k_threshold = 0;

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  wifiConnect();
  httpBeginPush();
  httpBegin();

  dhtBegin();
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

      if (value < THRESHOLD_MIN)
      {
        sendPush("Напряжение ниже 185");
      }

      if (value < THRESHOLD) // счетчик минимального напряжения
      {
        k_threshold++;
      }

      k_volt++;

      if (k_volt == k_time_volt && value > min_limit && value < max_limit) // каждые 15 сек записывает в файл показания напряжения
      {
        setVoltValues(value, k_stat_5);

        k_stat_5++;
        k_volt = 0;
      }

      if (k_stat_5 == k_arr_5min) // 20 - каждые 5 мин
      {
        k_stat++;
        handleVoltValues();
        k_stat_5 = 0;
      }

      if (k_stat == k_statistic) // 12 - каждый час - обрабатывает накопленные данные
      {
        setStatisticsData(k_threshold);
        k_stat = 0;
        k_threshold = 0;
      }
    }
  }

  delay(5000);
}
