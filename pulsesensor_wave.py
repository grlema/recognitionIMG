import pyqtgraph as pq
import numpy as np
import array
import serial  # 引用pySerial模組
import sys,time
from pyqtgraph.Qt import QtGui

COM_PORT = '/dev/cu.wchusbserial1450' #'/dev/cu.SLAB_USBtoUART'  # 'COM6'    # 指定通訊埠名稱
BAUD_RATES = 115200 #9600    # 設定傳輸速率

t = [0]
t_now = time.time()
t_start = t_now
#m = [0]
bpm = 0
ibi = 0
lasts = 0

app = QtGui.QApplication([]) #app = pq.mkQApp()

data=array.array('d')
t=array.array('d')
N=250

win = pq.GraphicsWindow()
win.setWindowTitle(u'心律波形圖')
win.resize(1000,500)

p=win.addPlot()
p.showGrid(x=True,y=True)
#p.setRange(xRange=[0,N-1],yRange=[-1.2,1.2],padding=0)
#test p.setRange(xRange=[N-1,0])
p.setLabels(left='y / v',bottom='x / time') #,title='BPM='+str(bpm))

#curve=p.plot(data,pen='y')


#test
for i in range(0,N):
    data.append(0)
    t.append(0)
curve=p.plot(data,pen='y')
#test
#curve=p.plot(t,data,pen='y')
#print(len(data),data)
#print(len(t),t)
ax = p.getAxis('bottom')  # This is the trick
dx = [(value, str('%.1f'%t[value])) for value in range(0,N,10)]
ax.setTicks([dx, []])

try:
    ser = serial.Serial(COM_PORT, BAUD_RATES)  # 初始化序列通訊埠
    ser.flushInput()

    #timer = pq.QtCore.QTimer()
    #timer.timeout.connect(plotData)
    #timer.start(1)

    while True:
        while ser.in_waiting:          # 若收到序列資料…
            data_raw = ser.readline()  # 讀取一行
            data_r = data_raw.decode()   # 用預設的UTF-8解碼,並刪除後'\r\n'
            data_r=data_r.replace('\r\n','')
            t_now = time.time()

            if data_r == '':
                break
            cmd=data_r[0]
            data_r = data_r[1:] #移除commad保留數字
            ##print(data_r)
            if str.isnumeric(data_r) == False:
                break
            if cmd =='S' or cmd == 's':
                s=int(data_r)
                lasts=s
            elif cmd == 'B' or cmd == 'b':
                bpm=int(data_r)
            elif cmd == 'Q' or cmd == 'q':
                ibi=int(data_r)
            #t.append(int(t_now-t_last))  # 模擬數據增量流入,保存歷史數據
            #m.append(lasts)  # 模擬數據增量流入,保存歷史數據
            if len(data)< N:
                data.append(lasts)
                t.append((t_now-t_start)*1)
            else:
                #test
                data[1:N] = data[0:N-1]
                data[0] = lasts
                t[1:N] = t[0:N-1]
                t[0] = (t_now-t_start)*1
                #test
                #data[:-1]=data[1:]
                #data[-1]=lasts
                #t[:-1]=t[1:]
                #t[-1]=(t_now-t_start)*1

            # 自定義 X軸的時間標示,波形左邊是最新值,右邊是最舊
            p.setLabels(title=f'BPM={str(bpm)}  IBI={str(ibi)} ms')
            dx = [(value, str('%.1f'%t[value])) for value in range(0,N,10)]
            ax.setTicks([dx, []])
            #print(data[0])
            #print(t)
            #print('dx=',dx)
            curve.setData(data)
            # for test : print(t[-1],data[-1])
            app.processEvents()

except KeyboardInterrupt:
#    plt.close()
    ser.flushInput()
    ser.close()    # 清除序列通訊物件
    print('再見！')
    sys.exit()
except(OSError, serial.SerialException):
    print('Serial Port Error!!')
    sys.exit()
except Exception as err:
    print(f'System Error:{err}')
    sys.exit()