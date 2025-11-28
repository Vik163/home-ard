// #include <WiFiClient.h>
// #include <ESP8266HTTPClient.h>

// #include <header.hpp>
// #include <credentialHttp.hpp>

// // Корневой сертификат
// WiFiClientSecure httpClient;
// X509List certRoot(IRG_Root_X1);

// HTTPClient https;

// /**
//  * функция добавления сертификата
//  */
// void setTrustAnchors()
// {
//    httpClient.setTrustAnchors(&certRoot);
// };

// bool httpBegin()
// {
//    if (https.begin(httpClient, url))
//    {
//       return true;
//    }
//    else
//       return false;
// }
// void httpEnd()
// {
//    https.end();
// }

// void getRequest()
// {

//    Serial.print("[HTTPS] GET...\n");
//    // start connection and send HTTP header
//    int httpCode = https.GET();

//    // httpCode will be negative on error
//    if (httpCode > 0)
//    {
//       // HTTP header has been send and Server response header has been handled
//       Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

//       // file found at server
//       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
//       {
//          String payload = https.getString();
//          Serial.println(payload);
//       }
//    }
//    else
//    {
//       Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
//    }
// }

// void postRequest()
// {
//    https.addHeader("Content-Type", "application/json");
//    int httpResponseCode = https.POST("{\"api_key\":\"aPnTH7Ab5\",\"sensor\":\"BME280\",\"value1\":\"36.00\",\"value2\":\"60.54\",\"value3\":\"954.14\"}");
// }
