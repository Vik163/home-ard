#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <header.hpp>

const char *fileMin = "min";
const char *fileMax = "max";
const char *fileAverage = "avr";
const char *fileThreshold = "thd";
const char *fileVolt = "vt";

int arrVolt[k_arr_5min]; // глобальный массив постоянного размера для хранения замеров напряжения

int minVal = 220;
int maxVal = 220;
int averageVal = 0;

JsonDocument doc;

// меняет значения в глобальном массиве
void setVoltValues(int value, int index)
{
   arrVolt[index] = value;
}

// меняет значения в глобальном массиве
void handleVoltValues()
{
   int avr = 0;
   minVal = 220;
   maxVal = 220;

   for (int i = 0; i < k_arr_5min; i = i + 1)
   {
      if (arrVolt[i])
      {
         if (arrVolt[i] < minVal)
         {
            minVal = arrVolt[i];
         }
         else if (arrVolt[i] > maxVal)
            maxVal = arrVolt[i];

         avr += arrVolt[i];
      }
   }

   if (averageVal == 0)
   {
      averageVal = avr / k_arr_5min;
   }
   else
      averageVal = (averageVal + avr / k_arr_5min) / 2;
}

// функция получет сведённое значение напряжения, массив json  и сохряняет в массив json (& - ссылка на массив, для получения всех его элементов)
void setStatisticsValues(const char *filename, int value, JsonArray &arr)
{
   int size_arr = arr.size(); // размер массива json

   if (arr && size_arr > 0) // если данные есть в массиве json
   {
      int newValueMin = 220;   // для max min
      int newValueMax = 220;   // для max max
      int newValueAverage = 0; // для среднего

      int k_min = 0;
      int k_max = 0;
      int k_avr = 0;
      int k_thd = 0;

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
            newValueAverage += val; // складываем для среднего значения счётчика предельного напряжения
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

         if (filename == fileMin)
         {
            arr[k_min] = value; // удаляем первый элемент и добавляем в конец новое значение
            k_min++;
            if (k_min == size_statics_array)
               k_min = 0;
         }
         else if (filename == fileMax)
         {
            arr[k_max] = value; // удаляем первый элемент и добавляем в конец новое значение
            k_max++;
            if (k_max == size_statics_array)
               k_max = 0;
         }
         else if (filename == fileAverage)
         {
            arr[k_avr] = value; // удаляем первый элемент и добавляем в конец новое значение
            k_avr++;
            if (k_avr == size_statics_array)
               k_avr = 0;
         }
         else if (filename == fileThreshold)
         {
            arr[k_thd] = value; // удаляем первый элемент и добавляем в конец новое значение
            k_thd++;
            if (k_thd == size_statics_array)
               k_thd = 0;
         }
      }
      else
      { // если размер массива меньше предельного
         for (int j = 0; j <= size_arr; j = j + 1)
         {
            if (j == size_arr)
            {
               arr[j] = value; // добавляем в конец массива новое значение
            }
         }
      }
   }
   else
   {
      // если массива нет создаем новый и сохраняем новое значение
      JsonArray arrJson = doc[filename].to<JsonArray>();
      arrJson.add(value);
   }
}

/**
 *  вызывается раз за долгий период
 * 1. обрабатывает массив с замеренным напряжением,
 * 2. отправляет запрос на сервер,
 * 3. получает данные json,
 * 4. отправляет в функцию для редактирования json все файлы с данными
 * 5. отправляет обработанные данные на сервер
 * @param счетчик количества предельных значений
 */
void setStatisticsData(int count_thd)
{
   String data = getRequest(); // нужно в базу загрузить первоначальные данные
   if (data != "error")
   {

      DeserializationError error = deserializeJson(doc, data); // десериализует json с проверкой на ошибку
      if (error)
      {
         Serial.print(F("deserializeJson() failed: "));
         Serial.println(error.c_str());
         return;
      }

      JsonArray arrMin = doc[fileMin]; // создаю ссылку на массив
      setStatisticsValues(fileMin, minVal, arrMin);

      JsonArray arrMax = doc[fileMax];
      setStatisticsValues(fileMax, maxVal, arrMax);

      JsonArray arrAverage = doc[fileAverage];
      setStatisticsValues(fileAverage, averageVal, arrAverage);

      JsonArray arrThd = doc[fileThreshold];
      setStatisticsValues(fileThreshold, count_thd, arrThd);

      // отправляю post - запрос
      if (doc[fileMin] && doc[fileMax] && doc[fileAverage] && doc[fileThreshold])
         postRequest(doc);

      doc.clear(); // очищаем json, чтобы освободить память
   }
}
