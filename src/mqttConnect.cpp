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
      Serial.print("Connecting to mqtt: ");
      mqttClient.setServer(mqttServer, mqttPort);

      if (mqttClient.connect(mqttClientId, mqttUser, mqttPass, mqttpTopicDiviceStatus, mqttpDiviceStatusQos, mqttpDiviceStatusRetained, mqttpDiviceStatusOff))
      {
         Serial.println("ok");
         mqttClient.publish(mqttpTopicDiviceStatus, mqttpDiviceStatusOn, mqttpDiviceStatusRetained);
         mqttClient.subscribe(mqttLoginTopic, mqttpDiviceStatusQos);

         mqttClient.publish(mqttLoginTopic, "no", mqttpDiviceStatusRetained);

         mqttClient.setCallback(mqttOnIncomingMsg);
      }
      else
      {
         Serial.print(" Mqtt failed, error code: ");
         Serial.print(mqttClient.state());
         Serial.println("!");
      }
      return mqttClient.connected();
   };
   return true;
}

/**
 * функция добавления сертификата
 */
void setTrustAnchors()
{
   wifiClient.setTrustAnchors(&certISRG);
};

void mqttPublish(const char *title, float value, const char *item, const char *topic)
{
   if (value != NAN)
   {
      // printf("%s%.1f %s\n", title, value, item);
   }
   else
      printf("Error reading: %s\n", title);

   String str_temp(value);
   mqttClient.publish(topic, str_temp.c_str(), mqttpDiviceStatusRetained);
}

void mqttPublishStr(const char *title, const char *value, const char *topic)
{
   // printf("%s%.1f %s\n", title, value, item);

   mqttClient.publish(topic, value, mqttpDiviceStatusRetained);
}

void mqttLoop()
{
   mqttClient.loop();
};
