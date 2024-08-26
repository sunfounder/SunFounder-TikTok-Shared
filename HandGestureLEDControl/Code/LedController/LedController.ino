/*
  This code controls a series of LEDs on an Arduino Uno based on finger count data 
  received from a Python script. The script detects the number of fingers shown 
  to a camera and sends this data to the Arduino via serial communication. The Arduino 
  then lights up the corresponding number of LEDs.

  The LEDs are connected to digital pins 2 through 6, with the number of lit LEDs 
  corresponding to the finger count detected by the Python script.

  Board: Arduino Uno R3 (or R4)
  Components: LEDs, Resistors

  Wulu from SunFounder 2024.08
*/

// Define the LED pins
const int ledPins[] = {2, 3, 4, 5, 6}; // Pins where the LEDs are connected
const int numLeds = 5; // Number of LEDs

void setup() {
  // Initialize the LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs are off initially
  }
  
  // Start serial communication
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {

    // Read the incoming data
    int value = Serial.readString().toInt();
    
    // Ensure the value is within the range 0-5
    if (value >= 0 && value <= numLeds) {
      // Turn off all LEDs first
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);
      }

      // Turn on the appropriate number of LEDs
      for (int i = 0; i < value; i++) {
        digitalWrite(ledPins[i], HIGH);
      }

    }
  }
}
