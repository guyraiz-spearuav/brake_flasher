#include <Arduino.h>
#include "mpu.h"
#include "control.h"
#include "output.h"

void setup()
{
    Serial.begin(115200);
    mpu_setup();
}

void loop()
{
    mpu_get_acc();
}