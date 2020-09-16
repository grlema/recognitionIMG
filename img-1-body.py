# img-1-body.py --input body/test.jpg
import numpy as np
import argparse
import cv2 as cv
 
parser = argparse.ArgumentParser()
parser.add_argument('--input', help='Path to image or video. Skip to capture frames from camera')
parser.add_argument('--thr', default=0.2, type=float, help='Threshold value for pose parts heat map')
parser.add_argument('--width', default=368, type=int, help='Resize input to specific width.')
parser.add_argument('--height', default=368, type=int, help='Resize input to specific height.')
 
args = parser.parse_args()
#COCO Output Format
BODY_PARTS = { "Nose": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
               "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
               "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "REye": 14,
               "LEye": 15, "REar": 16, "LEar": 17, "Background": 18 }
 
POSE_PAIRS = [ ["Neck", "RShoulder"], ["Neck", "LShoulder"], ["RShoulder", "RElbow"],
               ["RElbow", "RWrist"], ["LShoulder", "LElbow"], ["LElbow", "LWrist"],
               ["Neck", "RHip"], ["RHip", "RKnee"], ["RKnee", "RAnkle"], ["Neck", "LHip"],
               ["LHip", "LKnee"], ["LKnee", "LAnkle"], ["Neck", "Nose"], ["Nose", "REye"],
               ["REye", "REar"], ["Nose", "LEye"], ["LEye", "LEar"] ]

inWidth = args.width
inHeight = args.height
# Read the network into Memory
net = cv.dnn.readNetFromTensorflow("pb/graph_opt.pb")

cap = cv.VideoCapture(args.input if args.input else 0)
# 設定影像的尺寸大小
vWidth = 640
vHeight = 480
cap.set(cv.CAP_PROP_FRAME_WIDTH, vWidth)
cap.set(cv.CAP_PROP_FRAME_HEIGHT, vHeight)
#Read Camera's fps
#if cap.isOpened():
#    fps = cap.get(cv.CAP_PROP_FPS)
#else:
#    fps = 20
#print(fps)
# 使用 XVID 編碼
fourcc = cv.VideoWriter_fourcc(*'XVID')
# 建立 VideoWriter 物件，輸出影片至 body.avi
# FPS 值為 2.0，解析度為 640x360
outvideo = cv.VideoWriter('body.avi', fourcc, 3, (vWidth, vHeight))
 
while cv.waitKey(1) < 0:
    hasFrame, frame = cap.read()
    if not hasFrame:
        cv.waitKey()
        break
 
    frameWidth = frame.shape[1]
    frameHeight = frame.shape[0]
    
    net.setInput(cv.dnn.blobFromImage(frame, 1.0, (inWidth, inHeight), (127.5, 127.5, 127.5), swapRB=True, crop=False))
    out = net.forward()

    out = out[:, :19, :, :]  # MobileNet output [1, 57, -1, -1], we only need the first 19 elements
 
    assert(len(BODY_PARTS) == out.shape[1])
 
    points = []
    for i in range(len(BODY_PARTS)):
        # Slice heatmap of corresponging body's part.
        heatMap = out[0, i, :, :]

        # Originally, we try to find all the local maximums. To simplify a sample
        # we just find a global one. However only a single pose at the same time
        # could be detected this way.
        _, conf, _, point = cv.minMaxLoc(heatMap)
        x = (frameWidth * point[0]) / out.shape[3]
        y = (frameHeight * point[1]) / out.shape[2]
        # Add a point if it's confidence is higher than threshold.
        points.append((int(x), int(y)) if conf > args.thr else None)
 
    for pair in POSE_PAIRS:
        partFrom = pair[0]
        partTo = pair[1]
        assert(partFrom in BODY_PARTS)
        assert(partTo in BODY_PARTS)
 
        idFrom = BODY_PARTS[partFrom]
        idTo = BODY_PARTS[partTo]

        if points[idFrom] and points[idTo]:
            cv.line(frame, points[idFrom], points[idTo], (0, 255, 0), 2)
            cv.ellipse(frame, points[idFrom], (3, 3), 0, 0, 360, (0, 0, 255), cv.FILLED)
            cv.ellipse(frame, points[idTo], (3, 3), 0, 0, 360, (0, 0, 255), cv.FILLED)
            # print(idFrom, idTo, points[idTo], points[idTo])  # test
            px, py = points[idFrom]
            cv.putText(frame, '%1i' % idFrom, (px + 5, py), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0))
            px, py = points[idTo]
            cv.putText(frame, '%1i' % idTo, (px + 5, py), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0))

    #print(points) #test 骨架關節的座標值
 
    t, _ = net.getPerfProfile()
    freq = cv.getTickFrequency() / 1000
    px,py=(10,20)
    cv.putText(frame, '%.2fms' % (t / freq), (px, py), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0))
 
    cv.imshow('OpenPose using OpenCV', frame)
    # 寫入影像
    outvideo.write(frame)

cap.release()
outvideo.release()
cv.destroyAllWindows()