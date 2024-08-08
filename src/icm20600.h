#ifndef ICM20600_H
#define ICM20600_H

#include <Arduino.h>

class ICM20600 {
public:
    void begin(int sda, int scl);
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    uint8_t readRegister(uint8_t address, uint8_t reg);
    void readAccelData(int16_t* ax, int16_t* ay, int16_t* az);
    void readGyroData(int16_t* gx, int16_t* gy, int16_t* gz);
    bool isConnected();
    void calculateOdometry(float* distance, float* angle);

private:
    uint8_t ICM20600_ADDR = 0; // 添加 ICM20600_ADDR 变量
};

extern ICM20600 icm20600;
#endif // ICM20600_H