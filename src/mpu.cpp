#include <Arduino.h>
#include <Wire.h>
#include "MPU6050_tockn.h"
#include "mpu.h"
#include "control.h"

MPU6050 mpu6050(Wire);

float accel_y;

void mpu_setup()
{
  Wire.begin();
  mpu6050.begin();
}

void mpu_get_acc()
{
  mpu6050.update();
  accel_y = mpu6050.getAccY();
  value_from_mpu(accel_y);
}