#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <API.hpp>
#include <header.hpp>

const String url = "http://192.168.227.166/api/ard";

// Корневой сертификат
WiFiClient client;
HTTPClient http;

void postRequestServer(JsonDocument doc)
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

String getRequestServer()
{
   String payload;

   int httpCode = http.GET();

   // httpCode will be negative on error
   if (httpCode > 0)
   {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
         payload = http.getString();
         Serial.println(payload);
      }

      return payload;
   }
   else
   {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      return "error";
   }
}

bool serverBegin()
{
   if (http.begin(client, url))
   {
      String payload = getRequestServer();
      getResponseServer(payload);
      return true;
   }
   else
      return false;
}
void serverEnd()
{
   http.end();
}
