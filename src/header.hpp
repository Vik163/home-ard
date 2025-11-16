
bool wifiConnected(); // wifiConnect.cpp устанавливает соединение и синхронизирует время

bool mqttConnected();   // mqttConnect.cpp  устанавливает соединение с брокером
void setTrustAnchors(); // mqttConnect.cpp -> wifiConnect.cpp  добавляет сертификат
void mqttLoop();        // mqttConnect.cpp -> main.cpp  вызывается в цикле

void mqttPublish(const char *title, float value, const char *item, const char *topic); // mqttConnect.cpp -> dht.cpp, pzem.cpp
// отправляет данные брокеру

void dhtBegin();        // dht.cpp -> main.cpp  вызывается в setup
void readTemperature(); // dht.cpp -> main.cpp  вызывается в цикле

void readVoltage(); // pzem.cpp -> main.cpp  вызывается в цикле

// mqttControl.cpp -> mqttConnect.cpp Функция обратного вызова при поступлении входящего сообщения от брокера
void mqttOnIncomingMsg(char *topic, byte *payload, unsigned int length);

void setupPushTelegram();     // pushTelegram.cpp -> main.cpp
void sendMessageMinVoltage(); // pushTelegram.cpp -> pzem.cpp в setMinMaxValue()

// void reserveMemory();

void setMaxValue(float value); // pzwm.cpp -> mqttControl.cpp
void setMinValue(float value); // pzwm.cpp -> mqttControl.cpp

const char mqttpTopicAverage[] = "home/state/average";
const char mqttpTopicThreshold[] = "home/state/threshold";
const char mqttpTopicMax[] = "home/state/max";
const char mqttpTopicMin[] = "home/state/min";
