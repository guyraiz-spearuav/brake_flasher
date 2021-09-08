#include <Arduino.h>
#include "output.h"

const int LED_PIN = 10;

int my_led_brighness;


void pass_value_to_output(int led_brightness){
my_led_brighness = led_brightness;
}

void output_setup(){
  pinMode(LED_PIN, OUTPUT);
}

void output_do()
{
  digitalWrite(LED_PIN, my_led_brighness);
}