#include <PZEM004Tv30.h>
#include <TimeLib.h> // Для работы с временем (установите через Library Manager)
#include <SoftwareSerial.h>

#include <header.hpp>

const unsigned long READ_INTERVAL = 10000;                // 5 секунд (в мс)
const unsigned long ONE_DAY_MS = 24L * 60L * 60L * 1000L; // 24 часа в мс
const int THRESHOLD = 190;                                // Пороговое значение

// Буфер для хранения показаний (значение + время)
struct Reading
{
   unsigned long timestamp;
   int value;
};

float intMax;
float intMin;

std::vector<Reading> readingsThreshold;
std::vector<Reading> readingsAverage;
std::vector<Reading> readings;

// Переменные для таймера
unsigned long lastReadTime = 0;

PZEM004Tv30 pzem(D5, D6); // Software Serial pin 11 (RX) & 12 (TX)

const char mqttpTopicVoltage[] = "home/state/voltage";
const char mqttpTopicCurrent[] = "home/state/current";
const char mqttpTopicPower[] = "home/state/power";
const char mqttpTopicFrequency[] = "home/state/frequency";
const char mqttpTopicEnergy[] = "home/state/energy";
const char mqttpTopicPf[] = "home/state/pf";

void setMaxValue(float value)
{
   intMax = value;
   Serial.print("intMax ");
   Serial.print(intMax);
   Serial.println();
}

void setMinValue(float value)
{
   intMin = value;
   Serial.print("intMin ");
   Serial.print(intMin);
   Serial.println();
}

void setMinMaxValue()
{
   // Вычисляем min и max за последние 24 часа
   float minVal = 220;

   // if (intMin < 100)
   // {
   //    minVal = 220;
   // }
   // else
   // {
   //    minVal = intMin;
   // }

   float maxVal = 220;

   for (const auto &reading : readings)
   {
      if (reading.value < minVal)
         minVal = reading.value;
      if (reading.value > maxVal)
         maxVal = reading.value;
   }

   // Если буфер пуст, устанавливаем значения по умолчанию
   if (readings.empty())
   {
      minVal = 220;
      maxVal = 220;
   }
   mqttPublish("Max U for 24h: ", maxVal, "V", mqttpTopicMax);
   mqttPublish("Min U for 24h: ", minVal, "V", mqttpTopicMin);
}

void setTimeThreshold()
{
   // Считаем суммарное время, когда значение ≤ THRESHOLD
   unsigned long totalTimeBelowThreshold = 0;
   for (size_t i = 0; i < readingsThreshold.size(); ++i)
   {
      if (readingsThreshold[i].value <= THRESHOLD)
      {
         // Если это первое показание или предыдущее тоже ≤ THRESHOLD, добавляем интервал
         if (i == 0 || readingsThreshold[i - 1].value <= THRESHOLD)
         {
            unsigned long interval = (i == 0) ? READ_INTERVAL : min(READ_INTERVAL, readingsThreshold[i].timestamp - readingsThreshold[i - 1].timestamp);
            totalTimeBelowThreshold += interval;
         }
      }
   }

   // Переводим миллисекунды в часы и минуты
   unsigned long totalMinutes = totalTimeBelowThreshold / 60000;

   mqttPublish("totalMinutes U < 190v: ", totalMinutes, "m", mqttpTopicThreshold);
}

void setAverageVoltage(float voltage)
{
   unsigned long currentTime = millis();

   // Добавляем в буфер
   readingsAverage.push_back({currentTime, voltage});

   // Удаляем устаревшие показания (старше 24 часов)
   while (!readingsAverage.empty() && (currentTime - readingsAverage.front().timestamp > ONE_DAY_MS))
   {
      readingsAverage.erase(readingsAverage.begin());
   }

   // Вычисляем среднее
   float average = 0.0;
   if (!readingsAverage.empty())
   {
      long sum = 0;
      for (const auto &r : readingsAverage)
      {
         sum += r.value;
      }
      average = static_cast<float>(sum) / readingsAverage.size();
   }

   mqttPublish("For 24h: ", average, "V", mqttpTopicAverage);
   setTimeThreshold();
   setMinMaxValue();
}

void readVoltage()
{
   float voltage = pzem.voltage();
   mqttPublish("Voltage: ", voltage, "V", mqttpTopicVoltage);
   if (voltage < THRESHOLD)
      sendMessageMinVoltage(); // отправляет сообщение боту
   // setAverageVoltage(voltage);

   float current = pzem.current();
   mqttPublish("Current: ", current, "A", mqttpTopicCurrent);

   float power = pzem.power();
   mqttPublish("Power: ", power, "W", mqttpTopicPower);

   float energy = pzem.energy();
   mqttPublish("Energy: ", energy, "kWh", mqttpTopicEnergy);

   float frequency = pzem.frequency();
   mqttPublish("Frequency: ", frequency, "Hz", mqttpTopicFrequency);

   float pf = pzem.pf();
   mqttPublish("PowerFactor: ", pf, "", mqttpTopicPf);
}

// не работает  с такой конфигурацией (вместе с телеграмом)
// void reserveMemory()
// {
//    readingsThreshold.reserve(1000); // Резервируем память (примерно 17 280 записей за 24 ч при 5 с интервале)
//    readingsAverage.reserve(1000);   // Резервируем память (примерно 17 280 записей за 24 ч при 5 с интервале)
//    readings.reserve(1000);          // Резервируем память (примерно 17 280 записей за 24 ч при 5 с интервале)
// }
