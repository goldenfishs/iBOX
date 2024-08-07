#include <Arduino.h>
#include <SPIFFS.h>
#include "wifi_setup.h"
#include "display_setup.h"
#include "server_setup.h"
#include "spiffs_setup.h"  // 包含SPIFFS初始化头文件

char ssid[] = "wlang";
const char* password = "13750731785";

void setup() {
    Serial.begin(115200);

    // 初始化SPIFFS
    initSPIFFS();
    initWiFi(ssid, password);
    
    initDisplay();  // 使用正确的函数名
    // 启动HTTP服务器
    initServer();  // 使用正确的函数名
}

void loop() {
    // 主循环代码
}

// PNG解码回调函数
void pngle_draw_callback(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *rgba) {
  for (uint32_t i = 0; i < w; i++) {
    for (uint32_t j = 0; j < h; j++) {
      uint16_t color = tft.color565(rgba[0], rgba[1], rgba[2]);
      tft.drawPixel(x + i, y + j, color);
    }
  }
}