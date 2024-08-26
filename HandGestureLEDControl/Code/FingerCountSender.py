"""
This script captures video from a camera, detects the number of fingers shown using 
the MediaPipe Hands module, and sends the finger count to an Arduino via serial communication.

The script uses OpenCV to process the video feed and MediaPipe to perform hand and finger 
detection. The number of fingers detected is displayed on the video frame and sent to 
the Arduino in real-time.

Components:
- Camera: Captures the video feed.
- Arduino: Receives the finger count via serial communication and controls LEDs accordingly.

Serial Communication:
- Port: 'COM5' (Modify as needed)
- Baud Rate: 115200

Wulu from SunFounder 2024.08
"""

import cv2
import mediapipe as mp
import serial
import time
import math

# Set up serial communication with the Arduino
ser = serial.Serial('COM5', 115200, timeout=1)

# Initialize MediaPipe Hands module
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(min_detection_confidence=0.7)
mp_drawing = mp.solutions.drawing_utils

# Initialize camera
cap = cv2.VideoCapture(0)

def calculate_angle(p1, p2, p3):
    """ Calculate angle between three points. """
    v1 = (p1.x - p2.x, p1.y - p2.y)
    v2 = (p3.x - p2.x, p3.y - p2.y)
    dot_product = v1[0] * v2[0] + v1[1] * v2[1]
    magnitude_v1 = math.sqrt(v1[0]**2 + v1[1]**2)
    magnitude_v2 = math.sqrt(v2[0]**2 + v2[1]**2)
    cos_angle = dot_product / (magnitude_v1 * magnitude_v2)
    angle = math.degrees(math.acos(min(1.0, max(-1.0, cos_angle))))
    return angle

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    
    # Convert the frame to RGB
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    
    # Process the frame with MediaPipe Hands
    result = hands.process(rgb_frame)
    
    if result.multi_hand_landmarks:
        for hand_landmarks in result.multi_hand_landmarks:
            # Draw hand landmarks on the frame
            mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            
            # Count fingers based on landmark positions
            finger_tips_ids = [8, 12, 16, 20]
            count_fingers = sum([hand_landmarks.landmark[tip_id].y < hand_landmarks.landmark[tip_id - 3].y for tip_id in finger_tips_ids])
            
            # Calculate thumb angle and adjust finger count if the thumb is extended
            thumb_angle = calculate_angle(
                hand_landmarks.landmark[2],  # thumb base
                hand_landmarks.landmark[3],  # thumb middle
                hand_landmarks.landmark[4]   # thumb tip
            )
            if thumb_angle > 160:  # Adjust the angle threshold as needed
                count_fingers += 1
            
            # Display the number of fingers on the frame
            cv2.putText(frame, str(count_fingers), (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0, 255, 0), 3)
            print("finger:", count_fingers)

            # Send finger count to Arduino via serial communication
            ser.write(bytes(str(count_fingers), 'utf-8')) 
    else:
        ser.write(bytes('0', 'utf-8')) 

    # Show the frame
    cv2.imshow('Finger Count', frame)
    
    # Exit the loop when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
