import cv2
import numpy as np

# cap = cv2.VideoCapture(0)
#  畫布大小
img = np.zeros((720,1280,3), np.uint8)
# (位置),大小,(顏色)粗度
cv2.circle(img,(200,200), 100, (0,0,255), 2)
# (第一個座標)(第二個座標)(顏色),粗度
cv2.rectangle(img,(350,100),(550,300),(0,255,0),3)
# (第一個座標)(第二個座標)(顏色),粗度
cv2.line(img,(600,100),(800,300),(255,0,0),10)

pts = np.array([[10,5],[20,30],[70,20],[50,10]], np.int32)
pts = pts.reshape((-1,1,2))
cv2.polylines(img,[pts],True,(0,255,255))

font = cv2.FONT_HERSHEY_SIMPLEX
# (座標),大小,(顏色),粗度
cv2.putText(img,'Charlotte.HonG',(0,500), font, 5,(255,255,255),10)

while(1):
    cv2.imshow('Dring',img)
    if cv2.waitKey(20) & 0xFF == 27:
        break
# cap.release()
cv2.destroyAllWindows()