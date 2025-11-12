#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include "esp_sntp.h"

#include <header.hpp>
#include <credentialMqtt.hpp>

const char mqttpTopicDiviceStatus[] = "home/status";
const char *mqttpDiviceStatusOn = "online";
const char *mqttpDiviceStatusOff = "offline";
const int mqttpDiviceStatusQos = 1;
const bool mqttpDiviceStatusRetained = true;

// Корневой сертификат
BearSSL::X509List certISRG(ISRG_Root_x1);
BearSSL::WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

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

void setTrustAnchors()
{
   wifiClient.setTrustAnchors(&certISRG);
};

void mqttLoop()
{
   mqttClient.loop();
};

void mqttPublish(const char *topic, const char *payload, boolean retained)
{
   mqttClient.publish(topic, payload, retained);
   ;
};