#include <Arduino.h>
#include <Wire.h>
#include "MPU6050_tockn.h"
#include "mpu.h"
#include "control.h"

MPU6050 mpu6050(Wire);

const int FILTER_TIME = 100;

long unsigned int filter_timer = 0;
long unsigned int filter_counter = 0;
long double accumulated_accel_y;
double accel_y;
bool send_value;

void mpu_setup()
{
  Wire.begin();
  mpu6050.begin();
}

void mpu_get_acc()
{
  mpu6050.update();
  send_value = millis() - FILTER_TIME > filter_timer;
  if(!send_value){
    accumulated_accel_y += mpu6050.getAccY();
    filter_counter += 1;
  } else {
    accel_y = accumulated_accel_y / filter_counter;
    accumulated_accel_y = 0;
    filter_counter = 0;
    filter_timer = millis();
  }
  
  value_from_mpu(accel_y);
}