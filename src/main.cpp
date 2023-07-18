#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>

#include "sunny.h"

TFT_eSPI tft = TFT_eSPI();

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

void setup()
{
  Serial.begin(115200);
  Serial.print(F("Hello! Feather TFT Test"));

  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  tft.pushImage(1, 1, 240, 135, sunny);

  tft.drawString("Hello World!", 10, 10, 2);
  tft.drawString("This is Font2", 10, 24, 2);
  tft.setTextColor(TFT_RED);
  tft.drawString("Font4", 10, 40, 4);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  scanWiFi();
}

void loop()
{
}
