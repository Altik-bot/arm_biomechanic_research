import cv2
import numpy as np
import pandas as pd
import tensorflow as tf
import tensorflow_hub as hub
import os

# -----------------------------
# SETUP MODEL
# -----------------------------
model = hub.load("https://tfhub.dev/google/movenet/singlepose/thunder/4")
movenet = model.signatures["serving_default"]

KEYPOINTS = {
    "left_shoulder": 5,
    "left_elbow": 7,
    "left_wrist": 9
}

# -----------------------------
# INPUT VIDEO
# -----------------------------
video_path = "video4.mp4"

# -----------------------------
# CREATE DATA FOLDER
# -----------------------------
base_dir = "data"
os.makedirs(base_dir, exist_ok=True)

run_name = os.path.splitext(os.path.basename(video_path))[0]
run_dir = os.path.join(base_dir, run_name)
os.makedirs(run_dir, exist_ok=True)

# -----------------------------
# VIDEO CAPTURE
# -----------------------------
cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    print("Video not found")
    exit()

width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

# -----------------------------
# DETECTION FUNCTION
# -----------------------------
def detect(frame):
    img = cv2.resize(frame, (256, 256))
    img = img.astype(np.int32)
    img = np.expand_dims(img, axis=0)
    inputs = tf.convert_to_tensor(img)

    outputs = movenet(input=inputs)
    return outputs["output_0"].numpy()[0][0]

# -----------------------------
# EXTRACTION LOOP
# -----------------------------
data = []
frame_id = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    keypoints = detect(frame)

    ls = keypoints[KEYPOINTS["left_shoulder"]]
    le = keypoints[KEYPOINTS["left_elbow"]]
    lw = keypoints[KEYPOINTS["left_wrist"]]

    ls_x, ls_y = int(ls[1] * width), int(ls[0] * height)
    le_x, le_y = int(le[1] * width), int(le[0] * height)
    lw_x, lw_y = int(lw[1] * width), int(lw[0] * height)

    data.append([frame_id, ls_x, ls_y, le_x, le_y, lw_x, lw_y])
    frame_id += 1

cap.release()

# -----------------------------
# SAVE CSV
# -----------------------------
df = pd.DataFrame(data, columns=[
    "frame_id",
    "sh_x","sh_y",
    "el_x","el_y",
    "wr_x","wr_y"
])

output_path = os.path.join(run_dir, "raw_motion.csv")
df.to_csv(output_path, index=False)

print("Saved to:", output_path)