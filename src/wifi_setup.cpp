#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "wifi_setup.h"

void initWiFi(const char* ssid, const char* password) {
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