#include <TinyWireM.h>
#include <USI_TWI_Master.h>

// Connect SDA and SDC (ATTINY85 physical pins 5 & 7) to the MPU6050 and the SSD1306 OLED Display
// Connect Vcc/Gnd to MPU6050 and SSD1306 OLED Display  (if you are using the 128x64 display, comment out "setdisplay()"
// If you only want to use Serial Monitor OR OLED display, comment out calls to libraries, functions and routines which you don't require
//
// Tom Donnelly 2018



void get_accel(void);
void get_gyro(void);
void setup_MPU6050(void);

int accelX, accelY, accelZ;
int gyroX, gyroY, gyroZ;
char mpu = 0x68;  // I2C address of MPU.  Connect 5V to pin ADO to use 0x69 address instead


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
}

void loop() {
  float accel = accelY - ACCEL_OFFSET;
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



void setup_MPU6050() {
  TinyWireM.begin();
  // We need to do three things.  1. Disable sleep mode on the MPU (it activates on powerup).  2. Set the scale of the Gyro.  3. Set the scale of the accelerometer
  // We do this by sending 2 bytes for each:  Register Address & Value
  TinyWireM.beginTransmission(mpu);
  TinyWireM.write(0x6B); //  Power setting address
  TinyWireM.write(0b00000000); // Disable sleep mode (just in case)
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(mpu);
  TinyWireM.write(0x1B); // Config register for Gyro
  TinyWireM.write(0x00000000); // 250° per second range (defau<)
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(mpu); //I2C address of the MPU
  TinyWireM.write(0x1C); // Accelerometer config register
  TinyWireM.write(0b00000000); // 2g range +/- (defau<)
  TinyWireM.endTransmission();
}

void get_accel() {
  TinyWireM.beginTransmission(mpu); //I2C address of the MPU
  TinyWireM.write(0x3B); //  Acceleration data register
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(mpu, 6); // Get 6 bytes, 2 for each DoF
  accelX = TinyWireM.read() << 8; // Get X upper byte first
  accelX |= TinyWireM.read();     // lower
  accelY = TinyWireM.read() << 8; // Get Y upper byte first
  accelY |= TinyWireM.read();     // lower
  accelZ = TinyWireM.read() << 8; // Get Z upper byte first
  accelZ |= TinyWireM.read();     // lower
}

void get_gyro() {
  TinyWireM.beginTransmission(mpu); //I2C address of the MPU
  TinyWireM.write(0x43); // Gyro data register
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(mpu, 6); // Get 6 bytes, 2 for each DoF
  while (TinyWireM.available() < 6);
  gyroX = TinyWireM.read() << 8; // Get X upper byte first
  gyroX |= TinyWireM.read();     // lower
  gyroY = TinyWireM.read() << 8; // Get Y upper byte first
  gyroY |= TinyWireM.read();     // lower
  gyroZ = TinyWireM.read() << 8; // Get Z upper byte first
  gyroZ |= TinyWireM.read();     // lower
}
