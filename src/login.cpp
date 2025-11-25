#include <SoftwareSerial.h>

#include <header.hpp>

String loggedIn = "no";

/**
 * проверка пароля
 */
void setPassValue(const char *value)
{
   String data = mqttClientId + String(":") + userPass;
   Serial.print("VALUE PASS: ");
   Serial.println(value);

   if (data == value)
   {
      loggedIn = "yes";
      mqttPublishStr("Login: ", loggedIn.c_str(), mqttLoginTopic);
   }
}

void login()
{
   mqttPublishStr("Login: ", loggedIn.c_str(), mqttLoginTopic);
}