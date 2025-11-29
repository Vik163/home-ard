#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <header.hpp>

const char *fileMin = "min";
const char *fileMax = "max";
const char *fileAverage = "avr";
const char *fileThreshold = "thd";
const char *fileVolt = "vt";

int arrVolt[size_volt_array];
// int *newArrValue = new int[size_statics_array];

// // запись в файл с проверкой на лимиты
// void writeValue(const char *filename, int value)
// {
//    if (filename == fileThreshold && value >= 0 && value <= statistics_interval)
//       writeFile(filename, String(value).c_str()); // если не сравнялся дополняем
//    if (filename != fileThreshold && value > min_limit && value < max_limit)
//       writeFile(filename, String(value).c_str());
// }

void setVoltValues(int value, int index)
{
   arrVolt[index] = value;
}

int *setStatisticsValues(const char *filename, int value, JsonArray &arr)
{

   int size_arr = arr.size();
   int *newArrValue = new int[size_arr];

   Serial.print(F("VoltValues: "));
   Serial.println(value);

   // int size_new_array = sizeof(arrValues) / sizeof(arrValues);

   if (arr && size_arr > 0) // если данные есть в файле
   {
      Serial.print(F("size_new_array: "));
      Serial.println(size_arr);
      int newValueMin = 220;   // для max min
      int newValueMax = 220;   // для max max
      int newValueAverage = 0; // для среднего

      // Публикация MQTT =================================
      for (int i = 0; i < size_arr; i = i + 1)
      {
         int val = arr[i];
         Serial.print(F("val: "));
         Serial.println(val);

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
         mqttPublish("For 24h: ", newValueAverage / size_statics_array, "V", mqttpTopicAverage); // отправляю среднее значение
      }
      else if (filename == fileThreshold)
      {
         int totalMinutes = newValueAverage * 5 / 60; // перевел в минуты
         mqttPublish("totalMinutes U < 190v: ", totalMinutes, "m", mqttpTopicThreshold);
      }
      // ==================================================

      if (size_arr >= size_statics_array) // если размер массива сравнялся с пределом - меняем файлы с данными
      {
         for (int j = 0; j < size_statics_array; j = j + 1)
         {
            if (j + 1 < size_statics_array)
            {
               int val = arr[j + 1];
               newArrValue[j] = val; // заполняем новый массив сдвигая числа старого в начало на один
            }
            else
            {
               newArrValue[j] = value; // добавляем в конец массива новое значение
            }
         }
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
            else
            {
               // int v = arr[j].as<int>();
               // arr[j] = v; // заполняем новый массив сдвигая числа старого в начало на один
               // Serial.print(F("v: "));
               // Serial.println(v);
            }
         }
         // for (int j = 0; j < size_new_array; j = j + 1)
         // {
         //    Serial.print(F("newArrValue[j]final: "));
         //    Serial.println(newArrValue[j]);
         // }
      }
      delete newArrValue;
      return newArrValue;
   }
   else
   {
      JsonDocument doc;
      JsonArray arrJson = doc[filename].to<JsonArray>();

      arrJson.add(value);

      // Serial.print(F("arrMin[i]: "));
      // Serial.println(arrMin[i]);
      // newArrValue[0] = value;
      delete newArrValue;
      return newArrValue;
   }
}

/**
 *  вызывается раз за долгий период обрабатывает все файлы с данными
 * statistics.cpp
 */
void setStatisticsData()
{
   int minVal = 220;
   int maxVal = 220;
   int averageVal = 0;

   int size_array = sizeof(arrVolt) / sizeof(arrVolt);

   Serial.print(F("Size: "));
   Serial.println(size_array);

   for (int i = 0; i < size_array; i = i + 1)
   {
      Serial.print(F("arrVolt: "));
      Serial.println(arrVolt[i]);
      if (arrVolt[i] < minVal)
      {
         minVal = arrVolt[i];
      }
      else if (arrVolt[i] > maxVal)
         maxVal = arrVolt[i];

      averageVal += arrVolt[i];
   }
   String data = getRequestServer();
   if (data != "error")
   {
      JsonDocument doc;

      DeserializationError error = deserializeJson(doc, data);
      if (error)
      {
         Serial.print(F("deserializeJson() failed: "));
         Serial.println(error.c_str());
         // return;
      }

      JsonArray arrMin = doc[fileMin];
      int *arrMinVal = setStatisticsValues(fileMin, minVal, arrMin);

      // int *arrMax = setStatisticsValues(fileMax, maxVal, doc[fileMax]);

      // int *newArrAverage = new int[size_statics_array];
      // int *arrAverage = setStatisticsValues(fileAverage, averageVal / size_volt_array, doc[fileAverage], newArrAverage);
      // delete newArrAverage;

      // Serial.print(F("OLD POST: "));
      // serializeJson(doc, Serial);
      // Serial.println();

      // doc.clear();

      // int size_min = sizeof(arrMin) / sizeof(arrMin);
      // JsonArray digitalMin = doc[fileMin].to<JsonArray>();
      // for (int i = 0; i < size_min; i++)
      // {

      //    Serial.print(F("arrMin[i]: "));
      //    Serial.println(arrMin[i]);
      //    digitalMin.add(arrMin[i]);
      // }
      // // delete arrMin;

      // int size_max = sizeof(arrMax) / sizeof(arrMax);
      // JsonArray digitalMax = doc[fileMax].to<JsonArray>();
      // for (int i = 0; i < size_max; i++)
      // {
      //    digitalMax.add(arrMax[i]);
      // }
      // delete arrMax;

      // int size_average = sizeof(arrAverage) / sizeof(arrAverage);
      // JsonArray digitalAverage = doc[fileAverage].to<JsonArray>();
      // for (int i = 0; i < size_average; i++)
      // {
      //    digitalAverage.add(arrAverage[i]);
      // }
      // delete arrAverage;

      Serial.print(F("NEW POST: "));
      serializeJson(doc, Serial);
      Serial.println();
      if (doc[fileMin])
         postRequestServer(doc);
   }

   // delete arrVolt;
   // arrVolt = new int[size_volt_array];
}

/**
 *  вызывается раз за долгий период обрабатывает файл порогового значения
 * statistics.cpp
 */
void setTimeThreshold(int count)
{
   // setStatisticsValues(fileThreshold, count);
}
