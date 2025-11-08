#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// #include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// #include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include "esp_sntp.h"
#include <FS.h>

const char *wifiSSID = "realme C30";
const char *wifiPASS = "bwwjzwncgckq46c";
// const char *hostname = "ESP8266_1";

const char *mqttServer = "srv2.clusterfly.ru";
const char mqttClientId[] = "user_e3bbfff5_lamp";
const char mqttUser[] = "user_e3bbfff5";
const char mqttPass[] = "HVZj41dP31PZQ";
const int mqttPort = 9992;

const char mqttpTopicDiviceStatus[] = "home/status";
const char *mqttpDiviceStatusOn = "online";
const char *mqttpDiviceStatusOff = "offline";
const int mqttpDiviceStatusQos = 1;
const bool mqttpDiviceStatusRetained = true;

static const char ISRG_Root_x1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

#define DHTPIN D1 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
// #define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT11 // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)

// Корневой сертификат
BearSSL::X509List certISRG(ISRG_Root_x1);
BearSSL::WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// ESP8266WebServer server(80);

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

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
    wifiClient.setTrustAnchors(&certISRG);
  };
  return true;
}

bool mqttConnected()
{
  if (!mqttClient.connected())
  {
    Serial.print("Connecting to mqtt broker: ");
    mqttClient.setServer(mqttServer, mqttPort);

    if (mqttClient.connect(mqttClientId, mqttUser, mqttPass, mqttpTopicDiviceStatus, mqttpDiviceStatusQos, mqttpDiviceStatusRetained, mqttpDiviceStatusOff))
    {
      Serial.println("ok");
      mqttClient.publish(mqttpTopicDiviceStatus, mqttpDiviceStatusOn, mqttpDiviceStatusRetained);
    }
    else
    {
      Serial.print(" failed, error code: ");
      Serial.print(mqttClient.state());
      Serial.println("!");
    }
    return mqttClient.connected();
  };
  return true;
}

// void handleRoot()
// {
//   server.send(200, "text/plain", "hello from esp8266!");
// }

// void handleNotFound()
// {
//   String message = "File Not Found\n\n";
//   message += "URI: ";
//   message += server.uri();
//   message += "\nMethod: ";
//   message += (server.method() == HTTP_GET) ? "GET" : "POST";
//   message += "\nArguments: ";
//   message += server.args();
//   message += "\n";
//   for (uint8_t i = 0; i < server.args(); i++)
//   {
//     message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//   }
//   server.send(404, "text/plain", message);
// }

void setup(void)
{
  Serial.begin(9600);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, wifiPass);
  Serial.println();
  Serial.print("Проект для дома");

  // Wait for connection
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  // if (MDNS.begin("esp8266"))
  // {
  //   Serial.println("MDNS responder started");
  // }

  // server.on("/", handleRoot);

  // server.on("/inline", []()
  //           { server.send(200, "text/plain", "this works as well"); });

  // server.onNotFound(handleNotFound);

  // server.begin();
  // Serial.println("HTTP server started");

  // Serial.begin(9600);
  // // Initialize device.
  // dht.begin();
  // Serial.println(F("DHTxx Unified Sensor Example"));
  // // Print temperature sensor details.
  // sensor_t sensor;
  // dht.temperature().getSensor(&sensor);
  // Serial.println(F("------------------------------------"));
  // Serial.println(F("Temperature Sensor"));
  // Serial.print(F("Sensor Type: "));
  // Serial.println(sensor.name);
  // Serial.print(F("Driver Ver:  "));
  // Serial.println(sensor.version);
  // Serial.print(F("Unique ID:   "));
  // Serial.println(sensor.sensor_id);
  // Serial.print(F("Max Value:   "));
  // Serial.print(sensor.max_value);
  // Serial.println(F("°C"));
  // Serial.print(F("Min Value:   "));
  // Serial.print(sensor.min_value);
  // Serial.println(F("°C"));
  // Serial.print(F("Resolution:  "));
  // Serial.print(sensor.resolution);
  // Serial.println(F("°C"));
  // Serial.println(F("------------------------------------"));
  // // Print humidity sensor details.
  // dht.humidity().getSensor(&sensor);
  // Serial.println(F("Humidity Sensor"));
  // Serial.print(F("Sensor Type: "));
  // Serial.println(sensor.name);
  // Serial.print(F("Driver Ver:  "));
  // Serial.println(sensor.version);
  // Serial.print(F("Unique ID:   "));
  // Serial.println(sensor.sensor_id);
  // Serial.print(F("Max Value:   "));
  // Serial.print(sensor.max_value);
  // Serial.println(F("%"));
  // Serial.print(F("Min Value:   "));
  // Serial.print(sensor.min_value);
  // Serial.println(F("%"));
  // Serial.print(F("Resolution:  "));
  // Serial.print(sensor.resolution);
  // Serial.println(F("%"));
  // Serial.println(F("------------------------------------"));
  // // Set delay between sensor readings based on sensor details.
  // delayMS = sensor.min_delay / 1000;
}

void loop(void)
{

  if (wifiConnected())
  {
    if (mqttConnected())
    {
      mqttClient.loop();
    }
  }

  // Delay between measurements.
  // delay(delayMS);
  // // Get temperature event and print its value.
  // sensors_event_t event;
  // dht.temperature().getEvent(&event);
  // if (isnan(event.temperature))
  // {
  //   Serial.println(F("Error reading temperature!"));
  // }
  // else
  // {
  //   Serial.print(F("Temperature: "));
  //   Serial.print(event.temperature);
  //   Serial.println(F("°C"));
  // }
  // // Get humidity event and print its value.
  // dht.humidity().getEvent(&event);
  // if (isnan(event.relative_humidity))
  // {
  //   Serial.println(F("Error reading humidity!"));
  // }
  // else
  // {
  //   Serial.print(F("Humidity: "));
  //   Serial.print(event.relative_humidity);
  //   Serial.println(F("%"));
  // }
}
