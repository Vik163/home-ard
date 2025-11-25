# Types

## Аргументы функции

```c++
void func(
   const char *value, // строка
   float value, // число с запятой
   boolean value // булево значение
 )
```

## Типы определения

```c++
String str_temp(value); // String

PZEM004Tv30 pzem(D5, D6); // PZEM004Tv30 - класс, полует из установленной библиотеки

// #define определяется внутри файла в других не видна
#define DHTPIN D1 // название пина
#define DHTTYPE DHT11 // тип сенсора DHT11 или DHT 22 (AM2302)

struct tm timeinfo; // Структура (struct) в языке программирования C++ — это сложный тип данных, который позволяет объединить несколько переменных разных типов под одним именем. Это как «коробка» для хранения связанных данных.
// - группировка данных — связанная информация хранится вместе;
// - упрощение кода — вместо отдельных переменных работает один объект;
// - читаемость — код становится более понятным;
// - переиспользование — один раз объявили, много раз используем.


#include <string> // для string импортируется класс <string>

std::string loggedIn = "no";

String(value) // приводит int или float к String
String(value).c_str() // приводит int или float к const char *value

int convertedValue = atoi(value) // приводит char к int
float convertedValue = atof(value) // приводит char к float
```
