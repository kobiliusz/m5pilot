#include <ArduinoJson.h>
#include <M5StickC.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Ticker.h>
#include "creds.h"


HTTPClient http;
DynamicJsonDocument commands(4096);
char ip_str[16];
byte command_index = 0;
byte commands_size = 0;
Ticker timeout;

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
  IPAddress ip;
  int dns_code;
  dns_code = WiFi.hostByName(domain, ip);
  if (dns_code != 1) ip = MDNS.queryHost(domain);
  sprintf(ip_str, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  Serial.println(ip_str);
  send_command("/flows");
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
  reset_timer();
  
}

void loop() {

  M5.update();

  if (M5.BtnA.wasPressed()) {
    command_index = ++command_index % commands_size;
    redraw();
    reset_timer();
  }

  if (M5.BtnB.wasPressed()) {
    send_command(commands[command_index]["url"]);
    reset_timer();
  }
  
}

void reset_timer() {
  timeout.once(30, power_off);
}

void power_off() {
  M5.Axp.DeepSleep();
}

void send_command(const char* c_name){
  int httpCode;
  
  if (!http.begin(ip_str, 6666, c_name)) {
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
