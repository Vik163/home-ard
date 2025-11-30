#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <header.hpp>

const char *fileMin = "min";
const char *fileMax = "max";
const char *fileAverage = "avr";
const char *fileThreshold = "thd";
const char *fileVolt = "vt";

int arrVolt[size_volt_array];

JsonDocument doc;

void setVoltValues(int value, int index)
{
   arrVolt[index] = value;
}

void setStatisticsValues(const char *filename, int value, JsonArray &arr)
{

   int size_arr = arr.size();

   Serial.print(F("VoltValues: "));
   Serial.println(value);

   if (arr && size_arr > 0) // если данные есть в файле
   {
      int newValueMin = 220;   // для max min
      int newValueMax = 220;   // для max max
      int newValueAverage = 0; // для среднего

      // Публикация MQTT =================================
      for (int i = 0; i < size_arr; i = i + 1)
      {
         int val = arr[i];

         if (filename == fileMin && val < newValueMin) // добавил ограничения от багов
         {
            newValueMin = val; // выбираем меньшее значение
         }
         else if (filename == fileMax && val > newValueMax)
         {
            newValueMax = val; // выбираем большее значение
         }
         else if (filename == fileAverage)
         {
            newValueAverage += val; // складываем для среднего значения
         }
         else if (filename == fileThreshold)
         {
            newValueAverage += val; // складываем для среднего значения
         }
      }

      if (filename == fileMin)
      {
         mqttPublish("Min U for 24h: ", newValueMin, "V", mqttpTopicMin);
      }
      else if (filename == fileMax)
      {
         mqttPublish("Max U for 24h: ", newValueMax, "V", mqttpTopicMax);
      }
      else if (filename == fileAverage)
      {
         mqttPublish("For 24h: ", newValueAverage / size_arr, "V", mqttpTopicAverage); // отправляю среднее значение
      }
      else if (filename == fileThreshold)
      {
         int totalMinutes = newValueAverage * 5 / 60; // перевел в минуты
         mqttPublish("totalMinutes U < 190v: ", totalMinutes, "m", mqttpTopicThreshold);
      }
      // ==================================================

      if (size_arr >= size_statics_array) // если размер массива сравнялся с пределом - меняем файлы с данными
      {
         arr.remove(0);
         arr[size_arr - 1] = value;
      }
      else
      {
         for (int j = 0; j <= size_arr; j = j + 1)
         {
            if (j == size_arr)
            {
               arr[j] = value; // добавляем в конец массива новое значение
               Serial.print(F("value: "));
               Serial.println(value);
            }
         }
      }
   }
   else
   {
      // JsonDocument doc;
      JsonArray arrJson = doc[filename].to<JsonArray>();

      arrJson.add(value);
   }
}

/**
 *  вызывается раз за долгий период обрабатывает все файлы с данными
 * statistics.cpp
 */
void setStatisticsData(int count_thd)
{
   int minVal = 220;
   int maxVal = 220;
   int averageVal = 0;
   int k_length_avr = 0;

   // int size_array_volt = sizeof(&arrVolt) / sizeof(&arrVolt); только для динамических массивов

   for (int i = 0; i < size_volt_array; i = i + 1)
   {
      if (arrVolt[i])
      {
         if (arrVolt[i] < minVal)
         {
            minVal = arrVolt[i];
         }
         else if (arrVolt[i] > maxVal)
            maxVal = arrVolt[i];

         averageVal += arrVolt[i];
         k_length_avr++;
      }
   }
   String data = getRequestServer();
   if (data != "error")
   {

      DeserializationError error = deserializeJson(doc, data);
      if (error)
      {
         Serial.print(F("deserializeJson() failed: "));
         Serial.println(error.c_str());
         // return;
      }

      JsonArray arrMin = doc[fileMin];
      setStatisticsValues(fileMin, minVal, arrMin);

      JsonArray arrMax = doc[fileMax];
      setStatisticsValues(fileMax, maxVal, arrMax);

      JsonArray arrAverage = doc[fileAverage];
      setStatisticsValues(fileAverage, averageVal / k_length_avr, arrAverage);
      k_length_avr = 0;

      JsonArray arrThd = doc[fileThreshold];
      setStatisticsValues(fileThreshold, count_thd, arrThd);

      if (doc[fileMin] && doc[fileMax] && doc[fileAverage] && doc[fileThreshold])
         postRequestServer(doc);
   }
}
