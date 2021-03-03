
#include <M5StickC.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "creds.h"

HTTPClient http;

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
  int httpCode;
  
  if (!http.begin("192.168.0.214", 6666, "/flows")) {
    status_message();
    Serial.println("Http client error");
  }
  
  do {
    httpCode = http.GET();
    if (httpCode < 0) {
      Serial.println(http.errorToString(httpCode));
      delay(1000);
    }
  } while (httpCode != 0 && httpCode != HTTP_CODE_OK);
  
  Serial.println(http.getString());
  status_message();M5.Lcd.print(http.getString().c_str());
  
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
