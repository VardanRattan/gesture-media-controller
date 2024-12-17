#include <Wire.h>
#include <BleKeyboard.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <math.h>

// MPU-6050 Register Addresses
#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

// LED Indicator
#define LED_PIN 2  // Built-in LED on most ESP32 boards

// Global Bluetooth Device Name
const char* BLE_DEVICE_NAME = "Gesture Control";

// BLE Keyboard Instance
BleKeyboard bleKeyboard(BLE_DEVICE_NAME, "Vardan Rattan", 100);

// MPU6050 Tilt and Shake Detection Variables
float roll, pitch;
unsigned long lastShakeTime = 0;
unsigned long lastTiltTime = 0;

// Shake and Tilt Parameters
const float shakeThreshold = 1.5;  // Shake sensitivity
const int tiltCooldown = 1000;     // Cooldown time for tilt detection (in ms)
const float tiltDeadZone = 10.0;   // Threshold to prevent overlapping tilts
const int shakeDebounce = 500;     // Shake debounce time

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // LED Setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // MPU6050 Setup
  initializeMPU6050();

  // BLE Initialization
  BLEDevice::init(BLE_DEVICE_NAME);
  bleKeyboard.begin();

  Serial.println("Motion Media Controller Ready...");
  blinkLED(3, 200);
}

void initializeMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_PWR_MGMT_1);
  Wire.write(0);  // Wake up MPU6050
  Wire.endTransmission(true);
}

void loop() {
  int16_t rawX, rawY, rawZ;

  // Read Accelerometer Data
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);

  rawX = (Wire.read() << 8) | Wire.read();
  rawY = (Wire.read() << 8) | Wire.read();
  rawZ = (Wire.read() << 8) | Wire.read();

  // Convert to g's
  float xa = rawX / 16384.0;
  float ya = rawY / 16384.0;
  float za = rawZ / 16384.0;

  // Calculate Roll and Pitch
  roll = atan2(-ya, za) * 180.0 / PI;
  pitch = atan2(xa, sqrt(ya * ya + za * za)) * 180.0 / PI;

  // Detect Tilts
  detectTilts();

  // Detect Shakes
  detectShakes(xa, ya, za);

  delay(100);  // Adjust delay for responsiveness
}

void detectTilts() {
  if (millis() - lastTiltTime > tiltCooldown) {
    if (roll > 30 && abs(pitch) < tiltDeadZone) {
      Serial.println("Right Tilt - Next Track");
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
      blinkLED(1, 100);
      lastTiltTime = millis();
    } else if (roll < -30 && abs(pitch) < tiltDeadZone) {
      Serial.println("Left Tilt - Previous Track");
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
      blinkLED(1, 100);
      lastTiltTime = millis();
    }
  }
}

void detectShakes(float xa, float ya, float za) {
  // Prevent shake detection immediately after tilt detection
  if (millis() - lastTiltTime < tiltCooldown / 2) {  // Half of tilt cooldown
    return;                                          // Exit shake detection if a tilt was recently detected
  }

  // Calculate magnitude of acceleration
  float magnitude = sqrt(xa * xa + ya * ya + za * za);

  // Check shake condition with debounce
  if (magnitude > shakeThreshold && millis() - lastShakeTime > shakeDebounce) {
    Serial.println("Shake Detected - Play/Pause");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    blinkLED(2, 100);
    lastShakeTime = millis();
  }
}


void blinkLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
  }
}
