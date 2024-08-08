#include <Wire.h>
#include "icm20600.h"

// ICM-20600 寄存器地址
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43

#define ICM20600_ADDR 0x68 // 更新地址为 0x68

ICM20600 icm20600; // 定义变量

// 初始化 ICM-20600，允许自定义 I2C 引脚
void ICM20600::begin(int sda, int scl) {
    Wire.begin(sda, scl);
    delay(100); // 增加延迟

    // 读取 WHO_AM_I 寄存器以确认设备
    uint8_t whoAmI = readRegister(ICM20600_ADDR, WHO_AM_I);
    if (whoAmI == 0x11) { // WHO_AM_I 寄存器的默认值为 0x11
        Serial.print("ICM-20600 found at address 0x");
        Serial.println(ICM20600_ADDR, HEX);
    } else {
        Serial.println("ICM-20600 not found");
        return;
    }

    writeRegister(PWR_MGMT_1, 0x00); // 退出睡眠模式
    delay(100); // 增加延迟
    if (!isConnected()) {
        Serial.println("ICM-20600 not connected");
    } else {
        Serial.println("ICM-20600 connected");
    }
}

// 写寄存器
void ICM20600::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(ICM20600_ADDR);
    Wire.write(reg);
    Wire.write(value);
    if (Wire.endTransmission() != 0) {
        Serial.print("Error writing to register: ");
        Serial.println(reg, HEX);
    }
    delay(10); // 增加延迟
}

// 读寄存器
uint8_t ICM20600::readRegister(uint8_t reg) {
    Wire.beginTransmission(ICM20600_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) {
        Serial.print("Error writing to register: ");
        Serial.println(reg, HEX);
        return 0;
    }
    delay(10); // 增加延迟
    Wire.requestFrom(ICM20600_ADDR, 1);
    if (Wire.available()) {
        return Wire.read();
    } else {
        Serial.print("Error reading register: ");
        Serial.println(reg, HEX);
        return 0;
    }
}

// 读寄存器（带地址）
uint8_t ICM20600::readRegister(uint8_t address, uint8_t reg) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) {
        Serial.print("Error writing to register: ");
        Serial.println(reg, HEX);
        return 0;
    }
    delay(10); // 增加延迟
    Wire.requestFrom(address, 1);
    if (Wire.available()) {
        return Wire.read();
    } else {
        Serial.print("Error reading register: ");
        Serial.println(reg, HEX);
        return 0;
    }
}

// 读取加速度计数据
void ICM20600::readAccelData(int16_t* ax, int16_t* ay, int16_t* az) {
    Wire.beginTransmission(ICM20600_ADDR);
    Wire.write(ACCEL_XOUT_H);
    if (Wire.endTransmission() != 0) {
        Serial.println("Error writing to ACCEL_XOUT_H register");
        return;
    }
    delay(10); // 增加延迟
    Wire.requestFrom(ICM20600_ADDR, 6);
    if (Wire.available() == 6) {
        *ax = (Wire.read() << 8) | Wire.read();
        *ay = (Wire.read() << 8) | Wire.read();
        *az = (Wire.read() << 8) | Wire.read();
    } else {
        Serial.println("Error reading accelerometer data");
    }
}

// 读取陀螺仪数据
void ICM20600::readGyroData(int16_t* gx, int16_t* gy, int16_t* gz) {
    Wire.beginTransmission(ICM20600_ADDR);
    Wire.write(GYRO_XOUT_H);
    if (Wire.endTransmission() != 0) {
        Serial.println("Error writing to GYRO_XOUT_H register");
        return;
    }
    delay(10); // 增加延迟
    Wire.requestFrom(ICM20600_ADDR, 6);
    if (Wire.available() == 6) {
        *gx = (Wire.read() << 8) | Wire.read();
        *gy = (Wire.read() << 8) | Wire.read();
        *gz = (Wire.read() << 8) | Wire.read();
    } else {
        Serial.println("Error reading gyroscope data");
    }
}

// 检查设备是否连接
bool ICM20600::isConnected() {
    uint8_t whoAmI = readRegister(WHO_AM_I);
    Serial.print("WHO_AM_I register: ");
    Serial.println(whoAmI, HEX);
    return whoAmI == 0x11; // WHO_AM_I 寄存器的默认值为 0x11
}

// 计算里程计数据
void ICM20600::calculateOdometry(float* distance, float* angle) {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    readAccelData(&ax, &ay, &az);
    readGyroData(&gx, &gy, &gz);

    // 简单的里程计计算（假设每次调用间隔为1秒）
    // 这里的计算只是示例，实际应用中需要根据具体情况进行调整
    float accelX = ax / 16384.0; // 假设加速度计的量程为 ±2g
    float gyroZ = gz / 131.0;    // 假设陀螺仪的量程为 ±250°/s

    *distance += accelX * 9.81;  // 计算位移，假设每次调用间隔为1秒
    *angle += gyroZ;             // 计算角度变化，假设每次调用间隔为1秒
}