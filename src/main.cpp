#include <Arduino.h>
#include <SPIFFS.h>
#include "wifi_setup.h"
#include "display_setup.h"
#include "server_setup.h"
#include "spiffs_setup.h"  
#include "icm20600.h"
#include "tf_setup.h"


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
    initTFCard();
    // 启动HTTP服务器
    initServer();  // 使用A正确的函数名


}

void loop() {

}