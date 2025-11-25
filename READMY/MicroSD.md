# MicroSD

```c++
/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;

void initSd()
{
   Serial.print("Initializing SD card...");
   if (!SD.begin(15))
   {
      Serial.println("initialization failed!");
      return;
   }
   Serial.println("initialization done.");
}


bool existFile(const char *filename)
{
   bool isFile = SD.exists(filename);

   return isFile;
}

// записывает из массива тип int
void writeArrayFiles(const char *filename, int *arr)
{
   myFile = SD.open(filename, FILE_WRITE);
   if (myFile)
   {
      for (int j = 0; j < size_statics_array; j++)
      {
         myFile.println(String(arr[j]).c_str());
         printf("%s %d\n", filename, arr[j]);
      }
      myFile.close();
   }
   else
   {
      Serial.print("Error opening ");
      Serial.println(filename);
   }
}


void writeFile(const char *filename, const char *data)
{
   myFile = SD.open(filename, FILE_WRITE);
   if (myFile)
   {
      myFile.println(data);
      // close the file:
      myFile.close();
      printf("%s%s %s\n", "Writing to ", filename, data);
      Serial.println("done.");
   }
   else
   {
      Serial.println("error opening test.txt");
   }
}

// считает количество записей в файле (для определения размера массива)
int getNumLineFile(const char *filename)
{
   File myFile = SD.open(filename);

   int line_count = 0;

   if (myFile)
   {
      while (myFile.available())
      {
         String str = myFile.readStringUntil('\n');
         if (str)
         {
            line_count++;
         }
         else
            Serial.println("Error line_count");
      }

      myFile.close();
   }

   return line_count;
}

// считывает записи и сохраняет их в массив (размер определяет функция getNumLineFile(filename)), который возвращает
int *readFileTypeInt(const char *filename, const int size_array)
{
   File myFile = SD.open(filename);

   int *arrValue = new int[size_array]; // размер массива, компилятор заложит инструкции, которые выделят память под этот массив в рамзмере 10*2 = 20 байтов. 10 элементов по 2 байта

   if (myFile)
   {
      int line_count = 0;
      while (myFile.available())
      {
         String line = myFile.readStringUntil('\n');
         if (line)
         {
            int num = atoi(line.c_str());
            arrValue[line_count] = num;
            line_count++;
         }
         else
            Serial.print("Error readFileTypeInt ");
      }

      myFile.close();
   }
   return arrValue;
}

void removeFile(const char *filename)
{
   bool isRemove = SD.remove(filename);
   if (isRemove)
   {
      Serial.print("Remove ");
      Serial.println(filename);
   }
   else
   {
      Serial.print("Not remove ");
      Serial.println(filename);
   }
}


char Bytes[10]; // создаём переменную для хранения байтов в формате char,

void readDataFromFile(const char *filename)
{
   File myFile = SD.open(filename);

   if (myFile)
   {
      int k = 0; // создаём переменную-итератор,

      while (myFile.available() && // пока файл не кончился,
             ((Bytes[k] = myFile.read()) != '\n'))
         ++k; // читаем байты из файла до символа новой строки,

      k = 0;

      myFile.close();
   }
}

void readFileTypeFirstChar(const char *filename)
{
   char c = 0;
   readDataFromFile(filename);

   c = Bytes[0]; // записываем первый байт в переменную char,

   Serial.print("c ");
   Serial.print(c);
   Serial.println();
}

void readFileTypeInt(const char *filename)
{
   int i = 0; // создаём переменную для чтения из файла

   readDataFromFile(filename);

   i = atoi(Bytes); // конвертируем в int,

   Serial.print("i ");
   Serial.print(i);
   Serial.println();
}

void readFileTypeFloat(const char *filename)
{
   float f = 0; // создаём переменную для чтения из файла

   readDataFromFile(filename);

   f = atof(Bytes); // конвертируем в float,

   Serial.print("f ");
   Serial.print(f);
   Serial.println();
}

void readFileTypeString(const char *filename)
{
   String s = "";

   File myFile = SD.open(filename); // открываем ранее созданный файл file.txt

   if (myFile)
   {
      while (myFile.available() && // пока файл не кончился,
             ((s += char(myFile.read()))))
         ;            // читаем байты и сразу конкатенируем в переменную String.
      myFile.close(); // Закрываем файл.
   }

   Serial.print("s ");
   Serial.print(s);
   Serial.println();
}

void readFileType(const char *filename)
{
   String buffer;

   File myFile = SD.open(filename);

   if (myFile)
   {
      while (myFile.available())
      {
         buffer = myFile.readStringUntil('\n');
      }

      myFile.close(); // Закрываем файл.
   }

   Serial.print("buffer ");
   Serial.print(buffer);
   Serial.println();
}

```
