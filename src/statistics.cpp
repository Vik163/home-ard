#include <SoftwareSerial.h>
#include <header.hpp>

const char *fileMin = "min.txt";
const char *fileMax = "max.txt";
const char *fileAverage = "avr.txt";
const char *fileThreshold = "thd.txt";

int checkFile(const char *filename)
{
   int size_array = 0;

   bool isFile = existFile(filename);

   if (isFile)
   {
      size_array = getNumLineFile(filename); // число линий массива
      if (size_array > size_statics_array)
      {
         removeFile(filename);
         size_array = 0;
      }
      return size_array;
   }
   else
      return size_array;
}

// запись в файл с проверкой на лимиты
void writeValue(const char *filename, int value)
{
   if (filename == fileThreshold && value >= 0 && value <= statistics_interval)
      writeFile(filename, String(value).c_str()); // если не сравнялся дополняем
   if (filename != fileThreshold && value > min_limit && value < max_limit)
      writeFile(filename, String(value).c_str());
}

void setStatisticsValues(const char *filename, int value)
{

   int size_array = checkFile(filename); // проверка файла наличие и переполнение (возвращает количество записей или 0)

   int *newArrValue = new int[size_array]; // новый массив на замену старого

   if (size_array > 0) // если данные есть в файле
   {
      int *arrValues = readFileTypeInt(filename, size_array); // читаем файл и получаем массив с числами
      if (arrValues)
      {
         int newValueMin = 220;   // для max min
         int newValueMax = 220;   // для max max
         int newValueAverage = 0; // для среднего

         // Публикация MQTT =================================
         for (int i = 0; i < size_array; i = i + 1)
         {
            if (filename == fileMin && arrValues[i] < newValueMin) // добавил ограничения от багов
            {
               newValueMin = arrValues[i]; // выбираем меньшее значение
            }
            else if (filename == fileMax && arrValues[i] > newValueMax)
            {
               newValueMax = arrValues[i]; // выбираем большее значение
            }
            else if (filename == fileAverage)
            {
               newValueAverage += arrValues[i]; // складываем для среднего значения
            }
            else if (filename == fileThreshold)
            {
               newValueAverage += arrValues[i]; // складываем для среднего значения
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
            mqttPublish("For 24h: ", newValueAverage / size_array, "V", mqttpTopicAverage); // отправляю среднее значение
         }
         else if (filename == fileThreshold)
         {
            int totalMinutes = newValueAverage * 5 / 60; // перевел в минуты
            mqttPublish("totalMinutes U < 190v: ", totalMinutes, "m", mqttpTopicThreshold);
         }
         // ==================================================

         if (size_array >= size_statics_array) // если размер массива сравнялся с пределом - меняем файлы с данными
         {
            removeFile(filename); // 1. удаляем файл с данными

            for (int j = 0; j < size_array; j = j + 1)
            {
               if (j + 1 < size_array)
               {
                  newArrValue[j] = arrValues[j + 1]; // заполняем новый массив сдвигая числа старого в начало на один
               }
               else
               {
                  newArrValue[j] = value; // добавляем в конец массива новое значение
               }
            }
            writeArrayFiles(filename, newArrValue); // создаём новый файл с новыми значениями
         }
         else
            // заполняет массив пока не полный
            writeValue(filename, value);
      }
      delete arrValues; // удаляет из памяти массив
   }
   else
      // первая строчка в файле
      writeValue(filename, value);

   delete newArrValue; // удаляет из памяти массив
}

/**
 *  вызывается раз за долгий период обрабатывает все файлы с данными
 * statistics.cpp
 */
void setStatisticsData()
{
   int size_array = checkFile(fileVolt); // проверка переполнения файла

   // сортирую сохранённые в файле значения напряжения
   if (size_array > 0)
   {
      int *arrValue = readFileTypeInt(fileVolt, size_volt_array);
      int minVal = 220;
      int maxVal = 220;
      int averageVal = 0;

      for (int i = 0; i < size_volt_array; i = i + 1)
      {
         if (arrValue[i] < minVal)
         {
            minVal = arrValue[i];
         }
         else if (arrValue[i] > maxVal)
            maxVal = arrValue[i];

         averageVal += arrValue[i];
      }
      setStatisticsValues(fileMin, minVal);
      setStatisticsValues(fileMax, maxVal);
      setStatisticsValues(fileAverage, averageVal / size_volt_array);

      removeFile(fileVolt);
      delete arrValue;
   }
}

/**
 *  вызывается раз за долгий период обрабатывает файл порогового значения
 * statistics.cpp
 */
void setTimeThreshold(int count)
{
   setStatisticsValues(fileThreshold, count);
}
