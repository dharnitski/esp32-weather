#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "sunny.h"
#include "secrets.h"

TFT_eSPI tft = TFT_eSPI();

const char *location = "Atlanta,US";
const char *units = "metric";
int port = 80;

///////please enter your sensitive data in the secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char api_key[] = SECRET_OPEN_WEATHER_KEY;

void scanWiFi()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
}

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  Serial.print(F("Hello! Feather TFT Test"));

  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  tft.pushImage(0, 0, 240, 135, sunny);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // scanWiFi();

  initWiFi();
}

WiFiClient wifi; // or WiFiClientSecure for HTTPS
HttpClient client = HttpClient(wifi, "api.openweathermap.org", port);

void getWeather()
{
  char path[256];
  snprintf(path, sizeof(path), "/data/2.5/weather?q=%s&mode=json&units=%s&appid=%s", location, units, api_key);
  client.get(path);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, response);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject weather_0 = doc["weather"][0];
  int weather_0_id = weather_0["id"];                // 721
  const char *weather_condition = weather_0["main"]; // "Haze"

  JsonObject main = doc["main"];
  float main_temp = main["temp"];             // 25.05
  float main_feels_like = main["feels_like"]; // 25.33
  float main_temp_min = main["temp_min"];     // 21.68
  float main_temp_max = main["temp_max"];     // 26.98
  int main_pressure = main["pressure"];       // 1018
  int main_humidity = main["humidity"];       // 66

  JsonObject sys = doc["sys"];
  int sys_type = sys["type"];               // 2
  long sys_id = sys["id"];                  // 2006620
  const char *sys_country = sys["country"]; // "US"

  const char *name = doc["name"]; // "Atlanta"

  char location[128];
  snprintf(location, sizeof(location), "%s, %s", name, sys_country);
  tft.drawString(location, 10, 10, 4);

  char current_temp[16];
  snprintf(current_temp, sizeof(current_temp), "%.1fC", main_temp);
  tft.drawString(current_temp, 70, 50, 4);

  char range_temp[32];
  snprintf(range_temp, sizeof(range_temp), "H:%.1fC     L:%.1fC", main_temp_max, main_temp_min);
  tft.drawString(range_temp, 10, 90, 4);
}

void loop()
{
  getWeather();
  delay(60000);
}
