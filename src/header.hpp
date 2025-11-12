
bool wifiConnected();
void setTrustAnchors();
bool mqttConnected();
void readTemperature();
void readVoltage();
void mqttLoop();
void dhtBegin();

void mqttPublish(const char *topic, const char *payload, boolean retained);
void readItem(const char *title, float value, const char *item, const char *topic);
