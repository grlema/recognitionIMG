import cv2
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# 選擇第二隻攝影機（0 代表第一隻、1 代表第二隻）。
#cap = cv2.VideoCapture(0)
# 設定影像的尺寸大小
#cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
#cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
#while(True):
  # 從攝影機擷取一張影像
#  ret, frame = cap.read()
  # 顯示圖片
#  cv2.imshow('frame', frame)
  # 若按下 q 鍵則離開迴圈
#  if cv2.waitKey(1) & 0xFF == ord('q'):
#    cv2.imwrite('test.png',frame)
#    break
# 釋放攝影機

img = cv2.imread('20190102-2.jpeg') #20190102-2.jpeg
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#偵測臉部
faces = face_cascade.detectMultiScale(gray,scaleFactor=1.08,minNeighbors=5,minSize=(32,32))
for(x,y,w,h) in faces:
      cv2.rectangle(img, (x,y),(x+w,y+h),(0,255,0),2)

cv2.namedWindow('img', cv2.WINDOW_NORMAL) #正常視窗大小
cv2.imshow('img',img)                     #秀出圖片
cv2.imwrite('test.jpg',img)               #保存圖片
cv2.waitKey(0)

#cap.release()
# 關閉所有 OpenCV 視窗
cv2.destroyAllWindows()
print(faces)