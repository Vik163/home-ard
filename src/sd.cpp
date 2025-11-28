// /*
//   SD card read/write

//   The circuit:
//    SD card attached to SPI bus as follows:
//  ** MOSI - pin 11
//  ** MISO - pin 12
//  ** CLK - pin 13
//  ** CS - pin 4
// */

// #include <SPI.h>
// #include <SD.h>

// #include <header.hpp>

// File myFile;

// void initSD()
// {
//    Serial.print("SD card... ");
//    if (!SD.begin(15))
//    {
//       Serial.println("initialization failed!");
//       return;
//    }
//    Serial.println(" done.");
// }

// bool existFile(const char *filename)
// {
//    bool isFile = SD.exists(filename);

//    return isFile;
// }

// /**
//  * записывает из массива тип int
//  * sd.cpp
//  */
// void writeArrayFiles(const char *filename, int *arr)
// {
//    myFile = SD.open(filename, FILE_WRITE);
//    if (myFile)
//    {
//       for (int j = 0; j < size_statics_array; j++)
//       {
//          myFile.println(String(arr[j]).c_str());
//          printf("%s %d\n", filename, arr[j]);
//       }
//       myFile.close();
//    }
//    else
//    {
//       Serial.print("Error opening ");
//       Serial.println(filename);
//    }
// }

// void writeFile(const char *filename, const char *data)
// {
//    myFile = SD.open(filename, FILE_WRITE);
//    if (myFile)
//    {
//       myFile.println(data);
//       // close the file:
//       myFile.close();
//       printf("%s %s\n", filename, data);
//    }
//    else
//    {
//       Serial.print("Error opening ");
//       Serial.println(filename);
//    }
// }

// /**
//  * считает количество записей в файле (для определения размера массива)
//  * sd.cpp
//  */
// int getNumLineFile(const char *filename)
// {
//    File myFile = SD.open(filename);

//    int line_count = 0;

//    if (myFile)
//    {
//       while (myFile.available())
//       {
//          String str = myFile.readStringUntil('\n');
//          if (str)
//          {
//             line_count++;
//          }
//          else
//             Serial.println("Error line_count");
//       }

//       myFile.close();
//    }

//    return line_count;
// }

// /**
//  * считывает записи и сохраняет их в массив (размер определяет функция getNumLineFile(filename)), который возвращает
//  * sd.cpp
//  */
// int *readFileTypeInt(const char *filename, const int size_array)
// {
//    File myFile = SD.open(filename);

//    int *arrValue = new int[size_array]; // размер массива, компилятор заложит инструкции, которые выделят память под этот массив в рамзмере 10*2 = 20 байтов. 10 элементов по 2 байта

//    if (myFile)
//    {
//       int line_count = 0;
//       while (myFile.available())
//       {
//          String line = myFile.readStringUntil('\n');
//          if (line)
//          {
//             int num = atoi(line.c_str());
//             arrValue[line_count] = num;
//             line_count++;
//          }
//          else
//             Serial.print("Error readFileTypeInt ");
//       }

//       myFile.close();
//    }
//    return arrValue;
// }

// void removeFile(const char *filename)
// {
//    bool isRemove = SD.remove(filename);
//    if (isRemove)
//    {
//       Serial.print("Remove ");
//       Serial.println(filename);
//    }
//    else
//    {
//       Serial.print("Not remove ");
//       Serial.println(filename);
//    }
// }
