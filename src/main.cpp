#include <Arduino.h>
#include <SPIFFS.h>
#include "wifi_setup.h"
#include "display_setup.h"
#include "server_setup.h"
#include "spiffs_setup.h"  // 包含SPIFFS初始化头文件
#include "icm20600.h"

char ssid[] = "wlang";
const char* password = "13750731785";
char ssid_self[] = "ESP32";
const char* password_self = "12345678";


void setup() {
    Serial.begin(115200);
    initDisplay();  
    // 初始化SPIFFS
    initSPIFFS();
    initWiFi(ssid, password, ssid_self, password_self);
    icm20600.begin(5, 4);
    // 启动HTTP服务器
    initServer();  // 使用正确的函数名
}

void loop() {
    // 主循环代码
}
