#include <M5StickC.h>
#include <WiFi.h>
#include "creds.h"


void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(8);
  status_message();
  M5.Lcd.printf("Connecting...\n");
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  status_message();
  M5.Lcd.printf("Connected!\n");
  M5.Lcd.print("   IP ");
  M5.Lcd.print(WiFi.localIP());
  delay(1000);
  status_message();
  M5.Lcd.printf("Getting command list...\n");
}

void loop() {
  

}

void status_message() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 10, 2);
}

void redraw() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(0);
  M5.Lcd.setCursor(5, 0, 2);

}
