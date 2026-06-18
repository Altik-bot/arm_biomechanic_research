import cv2
import mediapipe as mp
import pandas as pd
video_path = "WhatsApp Video 2026-06-16 at 8.42.52 PM.mp4"
cap = cv2.VideoCapture(video_path)
mp_pose = mp.solutions.pose
pose = mp_pose.Pose()
data = []
frame_id = 0
print("start")
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
h, w, _ = frame.shape
rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

results = pose.process(rgb)

if results.pose_landmarks:
    lm = results.pose_landmarks.landmark

    sh = lm[12]
    el = lm[14]
    wr = lm[16]

    data.append([
        frame_id,
        frame_id * (1000 / 30),
        sh.x * w, sh.y * h,
        el.x * w, el.y * h,
        wr.x * w, wr.y * h
    ])

frame_id += 1
cap.release()
df = pd.DataFrame(data, columns=[
"frame_id","time_ms",
"sh_x","sh_y",
"el_x","el_y",
"wr_x","wr_y"
])
df.to_csv("pose_raw.csv", index=False)