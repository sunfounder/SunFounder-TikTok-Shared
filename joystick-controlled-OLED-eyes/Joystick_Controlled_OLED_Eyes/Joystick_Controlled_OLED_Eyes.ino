/*
  This Arduino sketch creates a pair of interactive, animated eyes displayed on an
  OLED screen, controlled by a joystick. The eyes follow the joystick's movement, 
  allowing the eyeballs to move within a defined range, simulating the appearance of 
  following an object. The joystick button is used to trigger a blinking animation, 
  where the upper part of the eyes is covered, mimicking a natural blink.

  Board: Arduino Uno
  Components: SSD1306 OLED Display, Joystick Module
  Library: https://github.com/adafruit/Adafruit_SSD1306 (Adafruit SSD1306 by Adafruit)  
           https://github.com/adafruit/Adafruit-GFX-Library (Adafruit GFX Library by Adafruit) 

  Wulu from SunFounder 2024.09
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin (not used, so set to -1)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int eyeAreaRadius = 18;     // Radius of the entire eye area
int pupilRadius = 6;        // Radius of the pupil
int reflectionRadius1 = 2;  // Radius of the first reflection spot
int reflectionRadius2 = 1;  // Radius of the second reflection spot
int eyeMovementMargin = 5;  // Margin to prevent the pupil from touching the eye edges

const int buttonPin = 2;  // Pin number for the joystick button
const int xPin = A0;      // Pin number for X-axis input from joystick
const int yPin = A1;      // Pin number for Y-axis input from joystick

bool isBlinking = false;  // Flag to track if the eyes are blinking

// Left eye parameters
int leftEyeX, leftEyeY;
int leftEyeCenterX = SCREEN_WIDTH / 2 - 20;  // X-coordinate of the left eye's center
int leftEyeCenterY = SCREEN_HEIGHT / 2;      // Y-coordinate of the left eye's center
int leftEyeMaxX = leftEyeCenterX + eyeAreaRadius - pupilRadius - eyeMovementMargin;
int leftEyeMinX = leftEyeCenterX - eyeAreaRadius + pupilRadius + eyeMovementMargin;
int leftEyeMaxY = leftEyeCenterY + eyeAreaRadius - pupilRadius - eyeMovementMargin;
int leftEyeMinY = leftEyeCenterY - eyeAreaRadius + pupilRadius + eyeMovementMargin;

// Right eye parameters
int rightEyeX, rightEyeY;
int rightEyeCenterX = SCREEN_WIDTH / 2 + 20;  // X-coordinate of the right eye's center
int rightEyeCenterY = SCREEN_HEIGHT / 2;      // Y-coordinate of the right eye's center
int rightEyeMaxX = rightEyeCenterX + eyeAreaRadius - pupilRadius - eyeMovementMargin;
int rightEyeMinX = rightEyeCenterX - eyeAreaRadius + pupilRadius + eyeMovementMargin;
int rightEyeMaxY = rightEyeCenterY + eyeAreaRadius - pupilRadius - eyeMovementMargin;
int rightEyeMinY = rightEyeCenterY - eyeAreaRadius + pupilRadius + eyeMovementMargin;

void setup() {
  pinMode(xPin, INPUT);              // Set pin A0 for X-axis input from joystick
  pinMode(yPin, INPUT);              // Set pin A1 for Y-axis input from joystick
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Stop if the display initialization fails
  }

  display.clearDisplay();  // Clear the display buffer
  display.display();       // Display the cleared buffer (turn off all pixels)
}

void loop() {
  int xValue = analogRead(xPin);             // Read the X-axis value from the joystick
  int yValue = analogRead(yPin);             // Read the Y-axis value from the joystick
  int buttonState = digitalRead(buttonPin);  // Read the state of the button

  // Map joystick values to left eye's pupil position within its allowed range
  leftEyeX = map(xValue, 0, 1023, leftEyeMinX, leftEyeMaxX);
  leftEyeY = map(yValue, 0, 1023, leftEyeMinY, leftEyeMaxY);

  // Map joystick values to right eye's pupil position within its allowed range
  rightEyeX = map(xValue, 0, 1023, rightEyeMinX, rightEyeMaxX);
  rightEyeY = map(yValue, 0, 1023, rightEyeMinY, rightEyeMaxY);

  // Check if the button is pressed to determine if the eyes should blink
  if (buttonState == LOW) {  // If the button is pressed
    isBlinking = true;
  } else {
    isBlinking = false;
  }

  display.clearDisplay();  // Clear the display buffer before drawing

  // Draw the left eye
  display.fillCircle(leftEyeCenterX, leftEyeCenterY, eyeAreaRadius, SSD1306_WHITE);  // Draw the eye area
  display.fillCircle(leftEyeX, leftEyeY, pupilRadius, SSD1306_BLACK);                // Draw the pupil
  display.fillCircle(leftEyeX - 2, leftEyeY - 2, reflectionRadius1, SSD1306_WHITE);  // Draw the first reflection
  display.fillCircle(leftEyeX + 1, leftEyeY + 1, reflectionRadius2, SSD1306_WHITE);  // Draw the second reflection

  // Draw the right eye
  display.fillCircle(rightEyeCenterX, rightEyeCenterY, eyeAreaRadius, SSD1306_WHITE);  // Draw the eye area
  display.fillCircle(rightEyeX, rightEyeY, pupilRadius, SSD1306_BLACK);                // Draw the pupil
  display.fillCircle(rightEyeX - 2, rightEyeY - 2, reflectionRadius1, SSD1306_WHITE);  // Draw the first reflection
  display.fillCircle(rightEyeX + 1, rightEyeY + 1, reflectionRadius2, SSD1306_WHITE);  // Draw the second reflection

  if (isBlinking) {
    // Draw the blinking effect (top half of the eyes black)
    display.fillRect(leftEyeCenterX - eyeAreaRadius, leftEyeCenterY - eyeAreaRadius, 2.5 * eyeAreaRadius, eyeAreaRadius, SSD1306_BLACK);
    display.fillRect(rightEyeCenterX - eyeAreaRadius, rightEyeCenterY - eyeAreaRadius, 2.5 * eyeAreaRadius, eyeAreaRadius, SSD1306_BLACK);
  }

  display.display();  // Update the display with the new drawings
  delay(50);          // Short delay to reduce flickering
}
