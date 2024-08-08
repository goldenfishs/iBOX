#include <Arduino.h>
#include <SPIFFS.h>

void listSPIFFSFiles() {
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.print("FILE: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }
}

void initSPIFFS() {
    // 初始化SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // 列出SPIFFS中的所有文件
    listSPIFFSFiles();

    // 检查是否存在 index.html 文件
    if (!SPIFFS.exists("/index.html")) {
        Serial.println("index.html not found, please upload it to SPIFFS.");
    } else {
        Serial.println("index.html found.");
    }
    if (!SPIFFS.exists("/styles.css")) {
        Serial.println("styles.css not found, please upload it to SPIFFS.");
    } else {
        Serial.println("styles.css found.");
    }
    if (!SPIFFS.exists("/scripts.js")) {
        Serial.println("scripts.js not found, please upload it to SPIFFS.");
    } else {
        Serial.println("scripts.js found.");
    }
}