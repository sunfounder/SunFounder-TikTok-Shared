/*
  This Arduino sketch controls an array of LEDs based on the tilt angle of an MPU6050 sensor. 
  The LEDs light up in response to the tilt detected on the X-axis, simulating a visual indicator 
  of the board's tilt orientation. The tilt angle is continuously calculated from the acceleration 
  data provided by the MPU6050, and the corresponding LED is activated according to predefined angle 
  thresholds.

  Board: Arduino Uno
  Components: MPU6050 Module, LEDs (7x)
  Library: https://github.com/electroniccats/mpu6050 (MPU6050 by Electronic Cats)

  Wulu from SunFounder 2024.10
*/


#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// LED pins 7-13 on Arduino
const int ledPins[] = {13, 12, 11, 10, 9, 8, 7}; 
int ledCount = sizeof(ledPins) / sizeof(ledPins[0]); // LED count

// Tilt angle thresholds for LEDs
const float thresholds[] = {-30, -15, -5, 5, 15, 30};

void setup() {
  Wire.begin(); // Init I2C
  Serial.begin(9600); // Init serial
  
  // Init MPU6050
  mpu.initialize(); 
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1); // Stop if MPU6050 fails
  }
  
  // Set LED pins as output
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  int16_t ax, ay, az; // Acceleration data
  
  // Get MPU6050 acceleration
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Calculate tilt angle (X-axis)
  float angleX = atan2(ay, az) * 180 / PI;
  
  // Update LEDs based on angle
  updateLEDs(angleX);
  
  delay(100); // Stabilize output
}

// Update LEDs based on angle
void updateLEDs(float angleX) {
  // Turn off all LEDs
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  // Get the LED index for the angle
  int ledIndex = getLEDIndex(angleX);

  // Turn on the correct LED
  if (ledIndex >= 0 && ledIndex < ledCount) {
    digitalWrite(ledPins[ledIndex], HIGH);
  }
}

// Determine the LED index for the angle
int getLEDIndex(float angleX) {
  // Find the matching LED
  for (int i = 0; i < ledCount - 1; i++) {
    if (angleX < thresholds[i]) {
      return i; // Return matching LED index
    }
  }
  return ledCount - 1; // Return last LED if angle exceeds thresholds
}
