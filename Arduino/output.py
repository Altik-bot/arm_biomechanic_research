import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

# -----------------------------
# SELECT INPUT
# -----------------------------
run_name = "video4"  # same name as video file
input_path = os.path.join("data", run_name, "raw_motion.csv")

df = pd.read_csv(input_path)

# -----------------------------
# CREATE RESULTS FOLDER
# -----------------------------
results_dir = os.path.join("results", run_name)
os.makedirs(results_dir, exist_ok=True)

# -----------------------------
# CLEAN + SMOOTH
# -----------------------------
def smooth(x, alpha=0.25):
    x = x.copy()
    for i in range(1, len(x)):
        x[i] = alpha * x[i] + (1 - alpha) * x[i-1]
    return x

for col in ["sh_x","sh_y","el_x","el_y","wr_x","wr_y"]:
    df[col] = smooth(df[col].to_numpy())

# -----------------------------
# METRICS
# -----------------------------
def velocity(x, y):
    return np.sqrt(np.diff(x)**2 + np.diff(y)**2)

wr_v = velocity(df["wr_x"], df["wr_y"])
wr_j = np.diff(np.diff(wr_v))

smoothness = np.mean(np.abs(wr_j))

# angle
def angle(a,b,c):
    ba = a - b
    bc = c - b
    cos = np.dot(ba, bc) / (np.linalg.norm(ba)*np.linalg.norm(bc))
    cos = np.clip(cos, -1, 1)
    return np.degrees(np.arccos(cos))

angles = []
for i in range(len(df)):
    S = np.array([df.iloc[i]["sh_x"], df.iloc[i]["sh_y"]])
    E = np.array([df.iloc[i]["el_x"], df.iloc[i]["el_y"]])
    W = np.array([df.iloc[i]["wr_x"], df.iloc[i]["wr_y"]])
    angles.append(angle(S,E,W))

df["elbow_angle"] = angles

# -----------------------------
# SAVE RESULTS
# -----------------------------
summary = pd.DataFrame([{
    "frames": len(df),
    "smoothness": smoothness,
    "mean_velocity": np.mean(wr_v),
    "mean_angle": np.mean(df["elbow_angle"])
}])

summary.to_csv(os.path.join(results_dir, "summary.csv"), index=False)

# plots
plt.plot(wr_v)
plt.title("Velocity")
plt.savefig(os.path.join(results_dir, "velocity.png"))
plt.clf()

plt.plot(wr_j)
plt.title("Jerk")
plt.savefig(os.path.join(results_dir, "jerk.png"))
plt.clf()

plt.plot(df["elbow_angle"])
plt.title("Angle")
plt.savefig(os.path.join(results_dir, "angle.png"))

print("Saved results to:", results_dir)