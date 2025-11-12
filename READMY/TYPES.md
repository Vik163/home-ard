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
```
