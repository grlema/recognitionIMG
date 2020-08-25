import cv2

# 載入分類器
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# 從視訊鏡頭擷取影片. 
# cap = cv2.VideoCapture(0)
# 使用現有影片
cap = cv2.VideoCapture('test.mp4')

while True:
    # Read the frame
    _, img = cap.read()

    # 轉成灰階
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # 偵測臉部
    faces = face_cascade.detectMultiScale(
    gray,
    scaleFactor=1.12,
    minNeighbors=4)

    # 繪製人臉部份的方框
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 255, 0), 2)

    # 顯示成果
    cv2.imshow('img', img)
    #計算找到幾張臉
    print("找到了 {0} 張臉.".format(len(faces)))

    # 按下ESC結束程式執行
    k = cv2.waitKey(30) & 0xff
    if k==27:
        break
# Release the VideoCapture object     
cap.release()
cv2.destroyAllWindows()
