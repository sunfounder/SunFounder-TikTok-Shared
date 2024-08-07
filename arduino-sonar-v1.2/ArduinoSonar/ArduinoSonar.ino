/*
  This code controls a servo motor to rotate an ultrasonic sensor and measure distances. 
  The sensor scans from 15 to 165 degrees and back, calculating the distance to objects 
  and sending the angle and distance data to the serial port. The data can be used in the 
  Processing IDE to visualize a real-time radar.

  An LED and buzzer are activated if an object is detected within 15 cm.
  
  Board: Arduino Uno R4 (or R3)
  Component: Ultrasonic distance Sensor(HC-SR04)

  Source Code From：https://howtomechatronics.com/projects/arduino-radar-project/

  Modified by wulu from SunFounder 2024.08

*/

#include <Servo.h>  // Include Servo library

// Define pins for the ultrasonic sensor
const int trigPin = 10;
const int echoPin = 11;

// Variables for measuring distance
long duration;
int distance;

const int servoPin = 12;
Servo myServo;  // Create a Servo object to control the motor

const int ledPin = 4;  // Pin for LED
const int buzzerPin = 5;  // Pin for Buzzer

void setup() {
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input

  pinMode(ledPin, OUTPUT);  // Set ledPin as an OUTPUT
  pinMode(buzzerPin, OUTPUT);  // Set buzzerPin as an OUTPUT

  Serial.begin(9600);
  myServo.attach(servoPin);  // Defines on which pin is the servo motor attached
}

void loop() {
  // rotates the servo motor from 15 to 165 degrees
  for (int i = 15; i <= 165; i++) {
    myServo.write(i);  // Set servo position
    delay(30);  // Wait for the servo to reach the position

    distance = calculateDistance();  // Measure distance at this angle
    handleAlerts();  // Check distance and handle alerts

    Serial.print(i);         // Sends the current degree into the Serial Port
    Serial.print(",");       // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance);  // Sends the distance value into the Serial Port
    Serial.print(".");       // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }

  // Repeats the previous lines from 165 to 15 degrees
  for (int i = 165; i > 15; i--) {
    myServo.write(i);
    delay(30);

    distance = calculateDistance(); // Measure distance at this angle
    handleAlerts();  // Check distance and handle alerts

    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
  }
}

// Function to read the sensor data and calculate the distance
int calculateDistance() {
  digitalWrite(trigPin, LOW);   // Set trig pin to low to ensure a clean pulse
  delayMicroseconds(2);         // Delay for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Send a 10 microsecond pulse by setting trig pin to high
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Set trig pin back to low

  // Measure the pulse width of the echo pin and calculate the distance value
  float distance = pulseIn(echoPin, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
  return distance;
}

void handleAlerts() {
  // Activate LED and buzzer if object is within 15 cm
  if (distance <= 15) {
    digitalWrite(ledPin, HIGH);  // Turn on LED
    tone(buzzerPin, 500);  // Turn on Buzzer with a 500 Hz tone
  } else {
    digitalWrite(ledPin, LOW);  // Turn off LED
    noTone(buzzerPin);  // Turn off Buzzer
  }
}