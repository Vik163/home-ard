
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include <header.hpp>
#include <credentialHttp.hpp>

// Корневой сертификат
WiFiClient client;
HTTPClient http;

// // Корневой сертификат
// WiFiClientSecure client;
// X509List certRoot(IRG_Root_X1);

// /**
//  * функция добавления сертификата
//  */
// void setTrustAnchors()
// {
//    client.setTrustAnchors(&certRoot);
// };

void postRequest(JsonDocument doc)
{
   Serial.print(F("Sending: "));
   serializeJson(doc, Serial);
   Serial.println();

   String output;
   serializeJson(doc, output);

   http.addHeader("Content-Type", "application/json");
   int httpResponseCode = http.POST(output);

   Serial.print("POST code... ");
   Serial.println(httpResponseCode);
}

String getRequest()
{
   String payload;
   int httpCode = http.GET();

   if (httpCode > 0)
   {
      // Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
         payload = http.getString();
         // Serial.println(payload);
      }
      return payload;
   }
   else
   {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      return "error";
   }
}

void httpBegin()
{
   if (http.begin(client, URL_DESK)) // URL_SERVER - сервер (https), URL_DESK - комп (http), URL_NOUT - ноут (http)
   {
      getRequest();
   }
}

bool httpConnected()
{
   if (http.connected())
   {
      return true;
   }
   else
      return false;
}
