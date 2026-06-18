import cv2
import pandas as pd
import numpy as np

# load your csv
df = pd.read_csv("movenet_output.csv")

# canvas size (adjust if needed)
width = 1000
height = 600

for i in range(len(df)):
    frame = np.zeros((height, width, 3), dtype=np.uint8)

    sh = (int(df.iloc[i]["sh_x"]), int(df.iloc[i]["sh_y"]))
    el = (int(df.iloc[i]["el_x"]), int(df.iloc[i]["el_y"]))
    wr = (int(df.iloc[i]["wr_x"]), int(df.iloc[i]["wr_y"]))

    # draw joints
    cv2.circle(frame, sh, 5, (0,255,0), -1)
    cv2.circle(frame, el, 5, (0,255,0), -1)
    cv2.circle(frame, wr, 5, (0,255,0), -1)

    # draw bones
    cv2.line(frame, sh, el, (255,0,0), 2)
    cv2.line(frame, el, wr, (255,0,0), 2)

    cv2.imshow("Reconstructed Motion", frame)

    if cv2.waitKey(30) & 0xFF == 27:
        break

cv2.destroyAllWindows()