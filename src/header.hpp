#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <stdint.h> // для определения uint8_t

void wifiConnect();     // wifiConnect.cpp устанавливает соединение и синхронизирует время
bool wifiIsConnected(); // возвращает true если связь установлена

void httpBegin();
bool httpConnected();
void postRequest(JsonDocument doc);
String getRequest();

void httpBeginPush();
void sendPush(String text);

bool mqttConnected(); // mqttConnect.cpp  устанавливает соединение с брокером
void mqttLoop();      // mqttConnect.cpp -> main.cpp  вызывается в цикле

void setTrustAnchors(); // httpConnect.cpp -> wifiConnect.cpp  добавляет сертификат

void mqttPublish(const char *title, float value, const char *item, const char *topic); // mqttConnect.cpp -> dht.cpp, pzem.cpp
void mqttPublishStr(const char *title, const char *value, const char *topic);          // mqttConnect.cpp -> login.cpp
// отправляет данные брокеру

void dhtBegin();        // dht.cpp -> main.cpp  вызывается в setup
void readTemperature(); // dht.cpp -> main.cpp  вызывается в цикле
int readVoltage();      // pzem.cpp -> main.cpp  вызывается в цикле

void setVoltValues(int value, int index);
void handleVoltValues();
void setStatisticsData(int count_thd); // statistics.cpp -> main.cpp  вызывается в цикле

// mqttControl.cpp -> mqttConnect.cpp Функция обратного вызова при поступлении входящего сообщения от брокера
// void mqttOnIncomingMsg(char *topic, uint8_t *payload, unsigned int length);

const char mqttpTopicAverage[] = "home/state/average";
const char mqttpTopicThreshold[] = "home/state/threshold";
const char mqttpTopicMax[] = "home/state/max";
const char mqttpTopicMin[] = "home/state/min";

const int k_time_volt = 3;         // 3 - каждые 3*5сек = 15 сек записывает в массив с показаниями напряжения
const int k_arr_5min = 20;         // 20 - каждые 20*15сек/60сек = 5мин перебирает массив и сохраняет обработанные данные в переменных
const int k_statistic = 12;        // 12 - каждый час в течении 24 часов (12*5мин значения в массиве) обновляет массивы на сервере
const int size_statics_array = 24; // 24 - 24 значения в массиве на сервере

const int THRESHOLD = 190;
const int THRESHOLD_MIN = 185;

const int min_limit = 150;
const int max_limit = 270;
