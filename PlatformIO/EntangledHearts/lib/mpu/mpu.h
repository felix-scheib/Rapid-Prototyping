#ifndef MPU_H
#define MPU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

struct Mpu {
    Mpu();

    void setup();
    bool update();

    sensors_event_t accel, gyro, temp;
    Adafruit_MPU6050 mpu;

    const static float THREASHOLD;
};



#endif
