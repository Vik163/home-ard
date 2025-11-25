#include <Arduino.h>

#include <header.hpp>

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

   // Сравниваем с топиками
   String _topic(topic);

   if (_topic.equals(mqttLoginTopic))
   {
      setPassValue(_payload.c_str());
   }
   else
   {
      Serial.println("Failed incoming topic!");
   };
}
