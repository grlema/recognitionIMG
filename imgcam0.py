import cv2
# 選擇第二隻攝影機（0 代表第一隻、1 代表第二隻）。
cap = cv2.VideoCapture(0)
# 設定影像的尺寸大小
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
while(True):
  # 從攝影機擷取一張影像
  ret, frame = cap.read()
  # 顯示圖片
  cv2.imshow('frame', frame)
  # 若按下 q 鍵則離開迴圈
  if cv2.waitKey(1) & 0xFF == ord('q'):
    cv2.imwrite('test.png',frame)
    break
# 釋放攝影機
cap.release()
# 關閉所有 OpenCV 視窗
cv2.destroyAllWindows()
#用cap.isOpened() 檢查攝影機是否有啟動
#用cap.open() 啟動它。