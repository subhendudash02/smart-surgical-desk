import cv2
import mediapipe as mp
import components
import requests
import threading

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

api_url = "https://smart-surgical-desk.vercel.app/api/hand_gesture"

# For webcam input:
cap = cv2.VideoCapture(0)
fingerCount = 0

def count_fingers():

  with mp_hands.Hands(
      model_complexity=0,
      min_detection_confidence=0.5,
      min_tracking_confidence=0.5) as hands:
    while cap.isOpened():
      success, image = cap.read()
      if not success:
        print("Ignoring empty camera frame.")
        # If loading a video, use 'break' instead of 'continue'.
        continue

      # To improve performance, optionally mark the image as not writeable to
      # pass by reference.
      image.flags.writeable = False
      image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
      results = hands.process(image)

      # Draw the hand annotations on the image.
      image.flags.writeable = True
      image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

      # Initially set finger count to 0 for each cap
      global fingerCount
      fingerCount = 0

      if results.multi_hand_landmarks:

        for hand_landmarks in results.multi_hand_landmarks:
          # Get hand index to check label (left or right)
          handIndex = results.multi_hand_landmarks.index(hand_landmarks)
          handLabel = results.multi_handedness[handIndex].classification[0].label

          # Set variable to keep landmarks positions (x and y)
          handLandmarks = []

          # Fill list with x and y positions of each landmark
          for landmarks in hand_landmarks.landmark:
            handLandmarks.append([landmarks.x, landmarks.y])

          # Test conditions for each finger: Count is increased if finger is 
          #   considered raised.
          # Thumb: TIP x position must be greater or lower than IP x position, 
          #   deppeding on hand label.
          if handLabel == "Left" and handLandmarks[4][0] > handLandmarks[3][0]:
            fingerCount = fingerCount+1
          elif handLabel == "Right" and handLandmarks[4][0] < handLandmarks[3][0]:
            fingerCount = fingerCount+1

          # Other fingers: TIP y position must be lower than PIP y position, 
          #   as image origin is in the upper left corner.
          if handLandmarks[8][1] < handLandmarks[6][1]:       #Index finger
            fingerCount = fingerCount+1
          if handLandmarks[12][1] < handLandmarks[10][1]:     #Middle finger
            fingerCount = fingerCount+1
          if handLandmarks[16][1] < handLandmarks[14][1]:     #Ring finger
            fingerCount = fingerCount+1
          if handLandmarks[20][1] < handLandmarks[18][1]:     #Pinky
            fingerCount = fingerCount+1

          # Draw hand landmarks 
          mp_drawing.draw_landmarks(
              image,
              hand_landmarks,
              mp_hands.HAND_CONNECTIONS,
              mp_drawing_styles.get_default_hand_landmarks_style(),
              mp_drawing_styles.get_default_hand_connections_style())

      # Display finger count
      cv2.putText(image, str(components.desk_components(fingerCount)), (50, 450), cv2.FONT_HERSHEY_SIMPLEX, 3, (255, 0, 0), 10)

      # Display image
      cv2.imshow('Smart Surgical Desk', image)
      if cv2.waitKey(5) & 0xFF == 27:
        break
  cap.release()


def send_api_requests():
    while True:
        if fingerCount > 0 and fingerCount < 4:
            requests.post(api_url, json={
                "finger": fingerCount, 
                "component": components.desk_components(fingerCount)})
        else:
            requests.post(api_url, json={
                "finger": 0,
                "component": "None"
            })

frame_thread = threading.Thread(target=count_fingers)
api_thread = threading.Thread(target=send_api_requests)

# Start the threads
frame_thread.start()
api_thread.start()

# Wait for the threads to finish
frame_thread.join()
api_thread.join()

# Clean up
cv2.destroyAllWindows()