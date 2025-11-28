
#include <ArduinoJson.h>

const char *PARAM_FILTER = "filter";

// void getAll(AsyncWebServerRequest *request)
// {
//   String message = "Get All";
//   Serial.println(message);
//   request->send(200, "text/plain", message);
// }

// void getFiltered(AsyncWebServerRequest *request)
// {
//   String message = "Get filtered by " + request->getParam(PARAM_FILTER)->value();
//   Serial.println(message);
//   request->send(200, "text/plain", message);
// }

// void getById(AsyncWebServerRequest *request)
// {
//   int id = GetIdFromURL(request, "/item/");

//   String message = String("Get by Id ") + id;
//   Serial.println(message);
//   request->send(200, "text/plain", message);
// }

// void getRequest(AsyncWebServerRequest *request)
// {

//   if (request->hasParam(PARAM_FILTER))
//   {
//     getFiltered(request);
//   }
//   else if (request->url().indexOf("/item/") != -1)
//   {
//     getById(request);
//   }
//   else
//   {
//     getAll(request);
//   }
// }

// int GetIdFromURL(AsyncWebServerRequest *request, String root)
// {
//   String string_id = request->url();
//   string_id.replace(root, "");
//   int id = string_id.toInt();
//   return id;
// }

JsonDocument getJsonContent(String data)
{
  JsonDocument doc;
  deserializeJson(doc, data);
  return doc;
}

void getResponseServer(String data)
{
  Serial.println(data);

  JsonDocument bodyContent = getJsonContent(data);
  serializeJsonPretty(bodyContent, Serial);

  // DeserializationError error = deserializeJson(doc, bodyContent);
  // if (error)
  // {
  //   Serial.print("ERROR Json... ");
  //   return;
  // }

  // String string_data = doc["data"];
  // String message = "Create " + string_data;
  // Serial.println(message);
}

// void postRequest(uint8_t *data, size_t len, size_t index, size_t total)
// {
//   String bodyContent = GetBodyContent(data, len);

//   StaticJsonDocument<200> doc;
//   DeserializationError error = deserializeJson(doc, bodyContent);
//   if (error)
//   {
//     Serial.print("ERROR Json... ");
//     return;
//   }

//   String string_data = doc["data"];
//   String message = "Create " + string_data;
//   Serial.println(message);

//   http.addHeader("Content-Type", "application/json");
//   int httpResponseCode = http.POST("{\"api_key\":\"aPnTH7Ab5\",\"sensor\":\"BME280\",\"value1\":\"36.00\",\"value2\":\"60.54\",\"value3\":\"954.14\"}");

//   Serial.print("POST code... ");
//   Serial.println(httpResponseCode);
// }

// void patchRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
// {
//   int id = GetIdFromURL(request, "/item/");
//   String bodyContent = GetBodyContent(data, len);

//   StaticJsonDocument<200> doc;
//   DeserializationError error = deserializeJson(doc, bodyContent);
//   if (error) { request->send(400); return;}

//   String string_data = doc["data"];
//   String message = String("Update ") + id + " with " + string_data;
//   Serial.println(message);
//   request->send(200, "text/plain", message);
// }

// void putRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
// {
//   int id = GetIdFromURL(request, "/item/");
//   String bodyContent = GetBodyContent(data, len);

//   StaticJsonDocument<200> doc;
//   DeserializationError error = deserializeJson(doc, bodyContent);
//   if (error) { request->send(400); return;}

//   String string_data = doc["data"];
//   String message = String("Replace ") + id + " with " + string_data;
//   Serial.println(message);
//   request->send(200, "text/plain", message);
// }

// void deleteRequest(AsyncWebServerRequest *request) {
//   int id = GetIdFromURL(request, "/item/");

//   String message = String("Delete ") + id;
//   Serial.println(message);
//   request->send(200, "text/plain", message);
// }
