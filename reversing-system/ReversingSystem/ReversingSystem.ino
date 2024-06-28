/*
  This Arduino sketch is designed for a distance measuring and alerting system using
  an ultrasonic sensor and a buzzer. It measures the distance to an object in front 
  of the sensor and sounds a buzzer at different frequencies based on the distance: 
  closer objects trigger higher frequency tones, and as the object moves away, the 
  frequency decreases. If no objects are detected within a minimal threshold distance 
  recently, it stops the buzzer.
  
  Board: Arduino Uno R4 (or R3)
  Component: Ultrasonic distance Sensor(HC-SR04) and Buzzer
*/

const int echoPin = 4;      // Ultrasonic sensor echo pin
const int trigPin = 3;      // Ultrasonic sensor trigger pin
const int buzzerPin = 9;    // Buzzer pin

unsigned long previousMillis = 0;  // Last time the buzzer was activated
unsigned long buzzerInterval = 0;  // Minimum interval between buzzer sounds

void setup() {
  Serial.begin(9600);            // Initialize serial communication at 9600 baud
  pinMode(echoPin, INPUT);       // Set echo pin as input
  pinMode(trigPin, OUTPUT);      // Set trigger pin as output
  pinMode(buzzerPin, OUTPUT);    // Set buzzer pin as output
}

void loop() {
  float distance = readDistance();  // Measure distance
  Serial.print(distance);           // Output distance to serial monitor
  Serial.println(" cm");            // Append units and new line

  manageBuzzer(distance);           // Adjust buzzer based on distance
}

void manageBuzzer(float distance) {
  unsigned long currentMillis = millis(); // Get current time

  // High frequency for close objects
  if (distance < 5 && currentMillis - previousMillis >= 100) {
    tone(buzzerPin, 1047, 100);    // Play tone
    previousMillis = currentMillis;
    buzzerInterval = 100;  
  } 
  // Medium frequency for medium distance
  else if (distance < 10 && currentMillis - previousMillis >= 300) {
    tone(buzzerPin, 523, 100);     // Play tone
    previousMillis = currentMillis;
    buzzerInterval = 300;  
  } 
  // Low frequency for distant objects
  else if (distance < 15 && currentMillis - previousMillis >= 500) {
    tone(buzzerPin, 262, 100);     // Play tone
    previousMillis = currentMillis;
    buzzerInterval = 500;  
  } 
  // Silence if no close objects detected recently
  else if (currentMillis - previousMillis >= buzzerInterval) {
    noTone(buzzerPin);             // Stop tone
  }
}

float readDistance() {
  digitalWrite(trigPin, LOW);       // Clear trigger pin
  delayMicroseconds(2);             // Stabilize line
  digitalWrite(trigPin, HIGH);      // Trigger pulse
  delayMicroseconds(10);            // Pulse duration
  digitalWrite(trigPin, LOW);       // End pulse
  float duration = pulseIn(echoPin, HIGH); // Read echo pulse
  return duration / 58.0;           // Convert to cm
}


