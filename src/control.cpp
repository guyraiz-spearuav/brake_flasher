#include <Arduino.h>
#include "control.h"
#include "output.h"

void control_do();
bool flash_led();

const int MIN_OUTPUT = 0;
const int MAX_OUTPUT = 255;
const int MIN_ACCEL_REQUIRED = 0;
const int MAX_ACCEL_REQUIRED = 500;
const int INTERVAL = 250;

float my_accel_y;
int led_brightness;
int flash_led_state = LOW;
unsigned long previous_millis = 0;

void value_from_mpu(float accel_y)
{
  my_accel_y = accel_y * 100;
  control_do();
}

void control_do()
{
  int brightness = map(my_accel_y, MIN_ACCEL_REQUIRED, MAX_ACCEL_REQUIRED, MIN_OUTPUT, MAX_OUTPUT);
  bool flash = my_accel_y >= MAX_ACCEL_REQUIRED;
  if (flash)
  {
    led_brightness = flash_led() * 255;
  }
  else
  {
    led_brightness = brightness;
    previous_millis = millis();
    flash_led_state = LOW;
  }
  pass_value_to_output(led_brightness);
}

bool flash_led()
{
  unsigned long current_millis = millis();
  if (current_millis - previous_millis >= INTERVAL)
  {
    previous_millis = current_millis;
    if (flash_led_state == LOW)
    {
      flash_led_state = HIGH;
    }
    else
    {
      flash_led_state = LOW;
    }
    return (flash_led_state);
  }
}