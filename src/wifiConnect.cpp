#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <header.hpp>
#include <credentialWifi.hpp>

bool wifiConnected()
{
   // Если подключение активно, то просто выходим и возвращаем true
   if (WiFi.status() != WL_CONNECTED)
   {
      // ... иначе пробуем подключиться к сети
      Serial.print("Connecting to WiFi AP ");
      Serial.print(wifiSSID);
      Serial.print(" ");

      // Настраиваем WiFi
      WiFi.mode(WIFI_STA);
      WiFi.begin(wifiSSID, wifiPASS);

      // И ждем подключения 60 циклов по 0,5 сек - это 30 секунд
      int i = 0;
      while (WiFi.status() != WL_CONNECTED)
      {
         i++;
         if (i > 60)
         {
            // Если в течение этого времени не удалось подключиться - выходим с false
            // Бескорнечно ждать подключения опасно - если подключение было разорвано во время работы
            // нужно всё равно "обслуживать" реле и датчики, иначе может случиться беда
            Serial.println("");
            Serial.println("Connection failed!");
            return false;
         };
         Serial.print(".");
         delay(500);
      };

      // Подключение успешно установлено
      Serial.println(" ок");
      Serial.print("WiFi connected, obtained IP address: ");
      Serial.println(WiFi.localIP());

      // Для работы TLS-соединения нужны корректные дата и время, получаем их с NTP серверов
      configTime("MSK+1", "pool.ntp.org", "time.nist.gov");
      // Ждем, пока локальное время синхронизируется
      Serial.print("Waiting for NTP time sync: ");
      i = 0;
      time_t now = time(nullptr);
      while (now < 1000000000)
      {
         now = time(nullptr);
         i++;
         if (i > 60)
         {
            // Если в течение этого времени не удалось подключиться - выходим с false
            // Бескорнечно ждать подключения опасно - если подключение было разорвано во время работы
            // нужно всё равно "обслуживать" реле и датчики, иначе может случиться беда
            Serial.println("");
            Serial.println("Time sync failed!");
            return false;
         };
         Serial.print(".");
         delay(500);
      }

      // Время успешно синхронизировано, выводим его в монитор порта
      // Только для целей отладки, этот блок можно исключить
      Serial.println(" ok");
      struct tm timeinfo;
      localtime_r(&now, &timeinfo);
      Serial.print("Current time: ");
      Serial.print(asctime(&timeinfo));

      // Теперь можно привязать корневой сертификат к клиенту WiFi
      setTrustAnchors();
   };
   return true;
}