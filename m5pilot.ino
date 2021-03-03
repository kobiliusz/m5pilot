#include <ArduinoJson.h>
#include <M5StickC.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "creds.h"


HTTPClient http;
DynamicJsonDocument commands(4096);
byte command_index = 0;
byte commands_size = 0;

void setup() {
  
  M5.begin();
  M5.Axp.ScreenBreath(15);
  
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

  String jsonString = http.getString();
  Serial.println(jsonString);
  
  status_message();
  M5.Lcd.printf("Parsing json...\n");
  deserializeJson(commands, jsonString);
  commands_size = commands.size();
  if (commands_size == 0) {
    Serial.println("No commands found!");
  }

  redraw();
  
}

void loop() {

  M5.update();

  if (M5.BtnA.wasPressed()) {
    command_index = ++command_index % commands_size;
    redraw();
  }

  if (M5.BtnB.wasPressed()) {
    send_command(commands[command_index]["url"]);
  }
  
}

void send_command(const char* c_name){
  
}

void status_message() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 10, 2);
}

void redraw() {
  M5.Lcd.fillScreen(TFT_DARKCYAN);
  M5.Lcd.setTextSize(0);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.println("  Menu");

  for (int i=0;i<commands_size;i++) {
    if (i==command_index) M5.Lcd.setTextColor(TFT_MAGENTA);
    else M5.Lcd.setTextColor(TFT_LIGHTGREY);
    const char* c_name = commands[i]["name"];
    M5.Lcd.print(" o ");
    M5.Lcd.println(c_name);
  }

}
