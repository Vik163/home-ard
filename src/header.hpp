#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <stdint.h> // для определения uint8_t
#include <credentialLogin.hpp>

void wifiConnect();     // wifiConnect.cpp устанавливает соединение и синхронизирует время
bool wifiIsConnected(); // возвращает true если связь установлена

void httpBegin();
bool httpConnected();
void postRequest(JsonDocument doc);
String getRequest();

bool mqttConnected(); // mqttConnect.cpp  устанавливает соединение с брокером
void mqttLoop();      // mqttConnect.cpp -> main.cpp  вызывается в цикле

void setTrustAnchors(); // httpConnect.cpp -> wifiConnect.cpp  добавляет сертификат

void mqttPublish(const char *title, float value, const char *item, const char *topic); // mqttConnect.cpp -> dht.cpp, pzem.cpp
void mqttPublishStr(const char *title, const char *value, const char *topic);          // mqttConnect.cpp -> login.cpp
// отправляет данные брокеру

void login();
void setPassValue(const char *value); // login.cpp -> mqttControl.cpp

// void initSD();
// bool existFile(const char *filename);
// void writeFile(const char *filename, const char *data);           // Делает запись в файле на SD
// void writeArrayFiles(const char *filename, int *arr);             // Делает записи из массива в файле на SD
// int getNumLineFile(const char *filename);                         // возвращает количество записей в файле
// int *readFileTypeInt(const char *filename, const int size_array); // возвращает динамический массив
// void removeFile(const char *filename);

void dhtBegin();        // dht.cpp -> main.cpp  вызывается в setup
void readTemperature(); // dht.cpp -> main.cpp  вызывается в цикле
int readVoltage();      // pzem.cpp -> main.cpp  вызывается в цикле

void setVoltValues(int value, int index);

void setStatisticsData(int count_thd); // statistics.cpp -> main.cpp  вызывается в цикле

// mqttControl.cpp -> mqttConnect.cpp Функция обратного вызова при поступлении входящего сообщения от брокера
// void mqttOnIncomingMsg(char *topic, uint8_t *payload, unsigned int length);

void setupPushTelegram();     // pushTelegram.cpp -> main.cpp
void sendMessageMinVoltage(); // pushTelegram.cpp -> pzem.cpp в setMinMaxValue()

const char mqttpTopicAverage[] = "home/state/average";
const char mqttpTopicThreshold[] = "home/state/threshold";
const char mqttpTopicMax[] = "home/state/max";
const char mqttpTopicMin[] = "home/state/min";

const int size_statics_array = 5; // каждые 20 минут в течении 24 часов (3*24=72 значения в массиве)
const int size_volt_array = 3;    // каждые 15 секунд в течении 20 минут (4*20=80 значения в массиве)
const int min_limit = 165;        // TODO использую в условиях ограничивая входящие значения если проблем не будет можно удалить
const int max_limit = 270;
