#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "wifi_setup.h"
#include "display_setup.h"

void initWiFi(const char* ssid, const char* password, const char* ssid_self, const char* password_self) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());

  // 获取外网 IP 地址
  String externalIP = getExternalIP();
  if (externalIP != "") {
    Serial.print("External IP Address: ");
    Serial.println(externalIP);
  } else {
    Serial.println("Failed to get External IP Address");
  }

  WiFi.softAP(ssid_self, password_self);

    //使用屏幕居中显示IP地址
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(0, 0);
  tft.print("IP Address:");
  tft.setCursor(0, 20);
  tft.print(WiFi.localIP().toString());
  tft.setCursor(0, 40);
  tft.print("IPV4 Address:");
  tft.setCursor(0, 60);
  tft.print(getExternalIP());
  tft.setCursor(0, 80);
  tft.print("SSID:");
  tft.print(ssid_self);
  tft.setCursor(0, 100);
  tft.print("Password:");
  tft.print(password_self);
}

String getExternalIP() {
  HTTPClient http;
  http.begin("http://api.ipify.org");
  int httpCode = http.GET();
  if (httpCode > 0) {
    String externalIP = http.getString();
    http.end();
    return externalIP;
  } else {
    http.end();
    return "";
  }
}
