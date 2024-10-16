/*
  This code controls the LED matrix on an Arduino Uno R4 WiFi board based on finger count data 
  received from a Python script. The script detects the number of fingers shown 
  to a camera and sends this data to the Arduino via serial communication(USB). The Arduino 
  then displays the corresponding number on the LED matrix.

  Board: Arduino Uno Uno R4 WiFi

  Wulu from SunFounder 2024.10
*/

#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

char lastChar = '\0';  // Variable to store the last read character

void setup() {
  // Start serial communication
  Serial.begin(115200);
  Serial.setTimeout(1);

  matrix.begin();

  delay(500);
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    // Read one character from serial input
    char c = Serial.read();

    // Check if the new character is different from the last character
    if (c != lastChar) {
      // Update the last character
      lastChar = c;

      // Clear the matrix before displaying the new character
      matrix.clear();

      matrix.beginDraw();

      matrix.stroke(0xFFFFFFFF);  // Set stroke color
      matrix.textFont(Font_5x7);  // Set font
      matrix.beginText(4, 1, 0xFFFFFF);  // Position and color for text
      matrix.print(c);  // Display the character
      matrix.endText();

      matrix.endDraw();
    }

    // Delay to prevent too frequent updates (optional)
    delay(10);
  }
}
