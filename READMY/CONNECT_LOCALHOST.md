# Подключение к localhost через WiFi

## 80 порт

1. Открыть командную строку (cmd) и ввести ipconfig, чтобы получить IP-адрес.

   ```java
   // Адаптер Ethernet Ethernet:

   // DNS-суффикс подключения . . . . . :
   // Локальный IPv6-адрес канала . . . : fe80::1386:5d66:575:318%13
   // IPv4-адрес. . . . . . . . . . . . : 192.168.0.15 - нужный ip адрес
   // Маска подсети . . . . . . . . . . : 255.255.255.0
   // Основной шлюз. . . . . . . . . : 192.168.0.1
   ```

2. Если не работает - в файле \Windows\system32\\drivers\etc\hosts добавить `127.0.0.1       192.168.0.15`
3. Если не работает - в конфигурационных файлах сервера (например, Apache, Nginx) указать ему прослушивать не только localhost, а и внешний IP-адрес. Обычно это настраивается в разделе настройки прослушивания (listening) или виртуальных хостов.

```c++
// Рабочий адрес `http://192.168.0.15/api/ard` `192.168.0.15/api/ard`


int HTTP_PORT = 80;
String HTTP_METHOD = "GET"; // or POST
char HOST_NAME[] = "192.168.0.15";
String PATH_NAME = "/api/ard";

WiFiClient client;

void wifiConnect()
{
// Connecting to WiFi

// connect to web server on port 80:
if (client.connect(HOST_NAME, HTTP_PORT))
{
   // if connected:
   Serial.println("Connected to server");
   // make a HTTP request:
   // send HTTP header
   client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
   client.println("Host: " + String(HOST_NAME));
   client.println("Connection: close");
   client.println(); // end HTTP header

   while (client.connected())
   {
      if (client.available())
      {
         // read an incoming byte from the server and print it to serial monitor:
         char c = client.read();
         Serial.print(c);
      }
   }

   // the server's disconnected, stop the client:
   client.stop();
   Serial.println();
   Serial.println("disconnected");
}
else
{ // if not connected:
   Serial.println("http failed");
}
}

```
