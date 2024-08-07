#include <Arduino.h>
#include <SPIFFS.h>
#include "wifi_setup.h"
#include "display_setup.h"
#include "server_setup.h"

char ssid[] = "wlang";
const char* password = "13750731785";

void setup() {
    Serial.begin(115200);

    // 初始化SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    // 读取src/index.html文件内容
    File srcFile = SPIFFS.open("/src/index.html", FILE_READ);
    if (!srcFile) {
        Serial.println("Failed to open source HTML file for reading");
        return;
    }
    
    // 写入新的HTML文件内容到SPIFFS
    File destFile = SPIFFS.open("/index.html", FILE_WRITE);
    if (!destFile) {
        Serial.println("Failed to open destination file for writing");
        srcFile.close();
        return;
    }
    
    while (srcFile.available()) {
        destFile.write(srcFile.read());
    }
    
    srcFile.close();
    destFile.close();

    // 初始化WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    initDisplay();  // 使用正确的函数名
    // 启动HTTP服务器
    initServer();  // 使用正确的函数名
}

void loop() {
    // 主循环代码
}