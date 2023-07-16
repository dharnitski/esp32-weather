#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(115200);
  Serial.print(F("Hello! Feather TFT Test"));

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(100);

  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
}

void loop()
{

  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_RED);
  delay(1000);
  // put your main code here, to run repeatedly:
}
