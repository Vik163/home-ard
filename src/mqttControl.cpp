#include <Arduino.h>
#include <string>

#include <header.hpp>

using namespace std;

/**
 * Функция обратного вызова при поступлении входящего сообщения от брокера
 */
void mqttOnIncomingMsg(char *topic, uint8_t *payload, unsigned int length)
{
   // Для более корректного сравнения строк приводим их к нижнему регистру и обрезаем пробелы с краев
   String _payload;
   for (unsigned int i = 0; i < length; i++)
   {
      _payload += String((char)payload[i]);
   };
   _payload.toLowerCase();
   _payload.trim();

   float value = std::stoi(_payload.c_str()); // привожу строку к числу типа float

   // Сравниваем с топиками
   String _topic(topic);
   if (_topic.equals(mqttpTopicMax))
   {
      setMaxValue(value);
   }
   else if (_topic.equals(mqttpTopicMin))
   {
      setMinValue(value);
   }
   else
   {
      Serial.println("Failed to recognize incoming topic!");
   };
}