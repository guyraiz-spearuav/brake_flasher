#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

//void flash();

const int LED_PIN = 9;
const int DEBOUNCE_DELAY_MS = 100;
const float MIN_G_REQ = 0.2;
const float ACCEL_OFFSET = 0.2;
const float MAX_BRIGHTNESS_G = 0.5;
const int LED_OFF = 0;
const int MAX_BRIGHTNESS = 255;
const int MIN_BRIGHTNESS = MAX_BRIGHTNESS / 3;
const int INTERVAL = 50;

float led_state = LED_OFF;
long unsigned int debounce_timer_on;
long unsigned int debounce_timer_off;
long unsigned long previousMillis = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  //  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();

  mpu6050.getAccX();
  mpu6050.getAccY();
  mpu6050.getAccZ();

  float accel = mpu6050.getAccY() - ACCEL_OFFSET;
  if (accel > MIN_G_REQ) {
    debounce_timer_off = millis();
  } else {
    debounce_timer_on = millis();
  }
  if (millis() - debounce_timer_on > DEBOUNCE_DELAY_MS) {
    if (accel > MAX_BRIGHTNESS_G) {
      flash();
    } else {
      led_state = map(accel * 1000, MIN_G_REQ * 1000, MAX_BRIGHTNESS_G * 1000, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }
  }
  if (millis() - debounce_timer_off > DEBOUNCE_DELAY_MS && led_state) {
    led_state = 0;
  }
  analogWrite(LED_PIN, led_state);
  Serial.println(led_state);
}

void flash() {
  if (millis() - previousMillis >= INTERVAL) {
    previousMillis = millis();
    if (led_state == MAX_BRIGHTNESS) {
      led_state = LED_OFF;
    } else {
      led_state = MAX_BRIGHTNESS;
    }
  }
}
