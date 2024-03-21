#include "mpu.h"

const float Mpu::THREASHOLD = 1.0f;

Mpu::Mpu() : 
    accel{}, gyro{}, temp{}, mpu{} {
}

void Mpu::setup() {
    while (!mpu.begin()) {
        Serial.println("Can't find mpu...");
        delay(500);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("Successfully initialized mpu");
}

bool Mpu::update() {
    mpu.getEvent(&accel, &gyro, &temp);

    if (gyro.gyro.y > THREASHOLD || gyro.gyro.y < -THREASHOLD) {
        return true;
    } else {
        return false;
    }
}