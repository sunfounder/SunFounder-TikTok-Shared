/*
  This code controls a servo motor to rotate an ToF distance sensor and measure distances.
  The sensor scans from 15 to 165 degrees and back, measuring the distance to objects
  and sending the angle and distance data to the serial port. The data can be used in the
  Processing IDE to visualize a real-time radar.

  Board: Arduino Uno R4 (or R3)
  Component: Time of Flight Distance Sensor (VL53L0X)
*/

// Variables for the duration and the distance
long duration;
int distance;

#include <Adafruit_VL53L0X.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Includes the Servo library
#include <Servo.h>

const int servoPin = 12;
Servo myServo; // Creates a servo object for controlling the servo motor

void setup()
{
  Serial.begin(9600);
  myServo.attach(servoPin); // Defines on which pin is the servo motor attached

  while (!Serial)
  {
    delay(1);
  }

  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }

}

void loop()
{
  // rotates the servo motor from 5 to 175 degrees
  for (int i = 5; i <= 175; i++)
  {
    myServo.write(i);
    delay(10);// Small delay for servo movement
    distance = getDistance(); //

    Serial.print(i);        // Sends the current degree into the Serial Port
    Serial.print(",");      // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance); // Sends the distance value into the Serial Port
    Serial.print(".");      // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }

  // Repeats the previous lines from 175 to 5 degrees
  for (int i = 175; i > 5; i--)
  {
    myServo.write(i);
    delay(10);
    distance = getDistance();

    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
  }
}

// Function to measure distance using VL53L0X
int getDistance() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  // If measurement is valid, return distance in cm; otherwise, return 0
  if (measure.RangeStatus != 4) {
    return measure.RangeMilliMeter / 10;  // Convert mm to cm
  } else {
    return 0;  // Out of range
  }
}