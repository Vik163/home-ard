
bool wifiConnected(); // wifiConnect.cpp устанавливает соединение и синхронизирует время

bool mqttConnected();   // mqttConnect.cpp  устанавливает соединение с брокером
void setTrustAnchors(); // mqttConnect.cpp -> wifiConnect.cpp  добавляет сертификат
void mqttLoop();        // mqttConnect.cpp -> main.cpp  вызывается в цикле

void mqttPublish(const char *title, float value, const char *item, const char *topic); // mqttConnect.cpp -> dht.cpp, pzem.cpp
// отправляет данные брокеру

void dhtBegin();        // dht.cpp -> main.cpp  вызывается в setup
void readTemperature(); // dht.cpp -> main.cpp  вызывается в цикле

void readVoltage(); // pzem.cpp -> main.cpp  вызывается в цикле
