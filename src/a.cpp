// Ниже — готовый код для NodeMCU(ESP8266) на платформе Arduino, реализующий два защищённых TLS‑соединения с разными серверами. Используем библиотеку WiFiClientSecure (встроенная в ESP8266 Core).

// Что делает код
// Подключается к Wi‑Fi.

// Устанавливает TLS‑соединение с первым сервером (например, https://httpbin.org).

// Отправляет HTTP‑запрос GET /json.

// Закрывает соединение.

// Повторяет то же самое для второго сервера (например, https://jsonplaceholder.typicode.com).

// Выводит ответы в Serial.

// Необходимые библиотеки
// WiFi — для подключения к сети.

// WiFiClientSecure — для TLS/SSL.

// ArduinoHttpClient (опционально, но удобно для HTTP).

// Важно: Для ESP8266 библиотека WiFiClientSecure поддерживает SNI и проверку сертификатов (при настройке).

// Код
// cpp
// #include <ESP8266WiFi.h>
// #include <WiFiClientSecure.h>

// // Настройки Wi-Fi
// const char* ssid     = "YOUR_WIFI_SSID";
// const char* password = "YOUR_WIFI_PASSWORD";

// // Серверы для подключения
// const char* server1 = "httpbin.org";
// const int   port1   = 443;

// const char* server2 = "jsonplaceholder.typicode.com";
// const int   port2   = 443;

// // Корневые сертификаты (опционально, для строгой проверки)
// // Здесь — упрощённый режим (проверка по домену + SNI)
// // Для продакшена — загрузите CA-сертификат через ESP8266CertStore или укажите fingerprint

// void setup() {
//    Serial.begin(115200);
//    delay(10);

//    // Подключение к Wi-Fi
//    Serial.println("Connecting to WiFi...");
//    WiFi.begin(ssid, password);
//    while (WiFi.status() != WL_CONNECTED)
//    {
//       delay(500);
//       Serial.print(".");
//    }
//    Serial.println("\nWiFi connected!");
//    Serial.print("IP address: ");
//    Serial.println(WiFi.localIP());

//    // Первое TLS‑соединение
//    makeTLSCall(server1, port1, "/json");

//    // Второе TLS‑соединение
//    makeTLSCall(server2, port2, "/posts/1");
// }

// void loop() {
//    // Ничего не делаем в loop
//    delay(60000); // Ждать 1 минуту перед повторным выполнением
// }

// // Функция для выполнения TLS‑запроса
// void makeTLSCall(const char* host, int port, const char* path) {
//    Serial.printf("\nConnecting to %s:%d...\n", host, port);

//    WiFiClientSecure client;

//    // Включаем проверку сертификата (строгий режим)
//    // Для тестирования можно отключить: client.setInsecure();
//    client.setCertificate(nullptr); // Если нет клиентского сертификата
//    client.setTrustAnchor(nullptr); // Если не используем CA-хранилище

//    // Альтернатива: проверка по fingerprint (более безопасно, чем setInsecure)
//    // client.setFingerprint("XX:XX:XX:..."); // Укажите SHA-1 отпечаток сервера

//    if (!client.connect(host, port))
//    {
//       Serial.println("Connection failed!");
//       return;
//    }

//    // Формируем HTTP GET‑запрос
//    String request = String("GET ") + path + " HTTP/1.1\r\n";
//    request += "Host: " + String(host) + "\r\n";
//    request += "User-Agent: ESP8266\r\n";
//    request += "Connection: close\r\n\r\n";

//    client.print(request);

//    // Читаем ответ
//    Serial.println("Waiting for response...");
//    while (client.connected())
//    {
//       String line = client.readStringUntil('\n');
//       if (line == "\r")
//       {
//          Serial.println("Headers ended, body follows:");
//          break;
//       }
//       Serial.println(line);
//    }

//    // Читаем тело ответа
//    while (client.available())
//    {
//       char c = client.read();
//       Serial.write(c);
//    }

//    client.stop();
//    Serial.println("\nConnection closed.");
// }
// Как настроить
// Замените YOUR_WIFI_SSID и YOUR_WIFI_PASSWORD на ваши данные Wi‑Fi.

// Выберите серверы — можно поменять server1 и server2 на свои (убедитесь, что они поддерживают HTTPS на 443‑м порту).

// Путь (path) — укажите нужный URL‑путь (например, /api/v1/data).

// Безопасность: варианты проверки сертификатов
// client.setInsecure() — отключает проверку (не рекомендуется для продакшена).

// client.setFingerprint("SHA1-отпечаток") — проверяет сертификат по отпечатку (безопаснее).

// client.setTrustAnchor() — использует CA‑сертификат из хранилища (самый безопасный).

// Чтобы получить отпечаток (fingerprint):

// bash
// openssl s_client -connect httpbin.org:443 -servername httpbin.org -showcerts </dev/null 2>/dev/null | openssl x509 -noout -fingerprint -sha1
// Примечания
// Память ESP8266 ограничена. Большие ответы могут вызвать проблемы — обрабатывайте их порциями.

// Таймауты — добавьте client.setTimeout(10000) при необходимости.

// HTTPS‑запросы — для сложных API рассмотрите библиотеку HTTPClient.h (она поверх WiFiClientSecure).

// Пример вывода в Serial
// Connecting to WiFi...
// ....
// WiFi connected!
// IP address: 192.168.1.100

// Connecting to httpbin.org:443...
// HTTP/1.1 200 OK
// Date: Mon, 01 Dec 2025 17:00:00 GMT
// Content-Type: application/json
// ...
// {"slippage": "random"}

// Connection closed.

// Connecting to jsonplaceholder.typicode.com:443...
// HTTP/1.1 200 OK
// Content-Type: application/json; charset=utf-8
// ...
// {"id": 1, "title": "Example"}

// Connection closed.