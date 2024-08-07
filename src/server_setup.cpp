#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include "server_setup.h"
#include "display_setup.h"
#include <pngle.h> // 确保包含 pngle 库的头文件
#include <WiFi.h> // 确保包含 WiFi 库的头文件

AsyncWebServer server(80);

void initServer() {
  server.on("/upload", HTTP_POST, handleTextUpload);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/upload_image", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Image upload complete");
  }, handleImageUpload);
  server.on("/performance", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"heap\":" + String(ESP.getFreeHeap()) + ",\"cpu\":" + String(ESP.getCpuFreqMHz()) + ",\"wifi\":" + String(WiFi.RSSI()) + ",\"uptime\":" + String(millis() / 1000) + "}";
    request->send(200, "application/json", json);
  });

  server.begin();
  Serial.println("HTTP server started");
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
}

void handleTextUpload(AsyncWebServerRequest *request) {
  if (request->hasParam("text", true)) {
    String text = request->getParam("text", true)->value();
    Serial.printf("Received text: %s\n", text.c_str());

    tft.fillScreen(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextWrap(true);

    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    int16_t x = (SCREEN_WIDTH - w) / 2;
    int16_t y = (SCREEN_HEIGHT - h) / 2;
    tft.setCursor(x, y);
    tft.print(text);

    request->send(200, "text/plain", "Upload complete");
  } else {
    request->send(400, "text/plain", "No text received");
  }
}

void handleImageUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
  static File file;

  if (index == 0) {
    Serial.printf("UploadStart: %s\n", filename.c_str());
    file = SPIFFS.open("/" + filename, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
  }

  if (file) {
    file.write(data, len);
  } else {
    Serial.println("Failed to open file for appending");
  }

  if (final) {
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
    file.close();
    
    file = SPIFFS.open("/" + filename, FILE_READ);
    if (file) {
      pngle_t *pngle = pngle_new();
      pngle_set_draw_callback(pngle, [](pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *rgba) {
        for (uint32_t i = 0; i < w; i++) {
          for (uint32_t j = 0; j < h; j++) {
            uint16_t color = tft.color565(rgba[0], rgba[1], rgba[2]);
            tft.drawPixel(x + i, y + j, color);
          }
        }
      });

      uint8_t buffer[1024];
      int len;
      while ((len = file.read(buffer, sizeof(buffer))) > 0) {
        if (pngle_feed(pngle, buffer, len) < 0) {
          Serial.printf("PNG decode error: %s\n", pngle_error(pngle));
          break;
        }
      }

      pngle_destroy(pngle);
      file.close();
    } else {
      Serial.println("Failed to open file for reading");
    }
  }
}