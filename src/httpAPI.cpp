
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include <header.hpp>
#include <credentialHttp.hpp>

HTTPClient http;
HTTPClient httpPush;

// Корневой сертификат
// WiFiClient client;
WiFiClientSecure client;
X509List certRoot(IRG_Root_X1);

/**
 * функция добавления сертификата
 */
void setTrustAnchors()
{
   client.setTrustAnchors(&certRoot);
};

void sendPush(String text)
{
   httpPush.addHeader("Content-Type", "text/plain");
   int httpResponseCode = httpPush.POST(text);

   Serial.print("PUSH code... ");
   Serial.println(httpResponseCode);
   httpPush.end(); // закрываем, чтобы освободить память (а то работает через раз)
}

void postRequest(JsonDocument doc)
{
   // Serial.print(F(""));
   // serializeJson(doc, Serial);
   // Serial.println();

   String output;
   serializeJson(doc, output);

   http.addHeader("Content-Type", "application/json");
   int httpResponseCode = http.POST(output);

   doc.clear(); // очищаем json, чтобы освободить память

   Serial.print("POST code... ");
   Serial.println(httpResponseCode);
   http.end(); // закрываем, чтобы освободить память (а то работает через раз)
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
   if (http.begin(client, URL_SERVER)) // URL_SERVER - сервер (https), URL_DESK - комп (http), URL_NOUT - ноут (http)
   {
      getRequest();
   }
}

void httpBeginPush()
{
   if (httpPush.begin(client, URL_SERVER_PUSH))
   {
      sendPush("Снова на связи!");
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
