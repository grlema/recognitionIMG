import serial  # 引用pySerial模組
import sys
from scipy import signal
import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
import numpy as np
import time
from math import *

COM_PORT = '/dev/cu.wchusbserial1450' #'/dev/cu.SLAB_USBtoUART'  # 'COM6'    # 指定通訊埠名稱
BAUD_RATES = 115200 #9600    # 設定傳輸速率

try:
    ser = serial.Serial(COM_PORT, BAUD_RATES)   # 初始化序列通訊埠

    ser.flushInput()
    plt.ion()           #開啟interactive mode 成功的關鍵函數
    plt.figure()
    t = [0]
    t_now = 0
    m = [0]
    plt.clf()
    bpm=0
    ibi=0
    lasts=0

    while True:
        while ser.in_waiting:          # 若收到序列資料…
            data_raw = ser.readline()  # 讀取一行
            data = data_raw.decode()   # 用預設的UTF-8解碼,並刪除後'\r\n'
            data=data.replace('\r\n','')

            if data == '':
                break
            cmd=data[0]
            data = data[1:] #移除commad保留數字
            # print(data)
            if str.isnumeric(data) == False:
                break
            if cmd =='S' or cmd == 's':
                #if data[1] <= '9' or data[1] >= '0':
                #if len(data)<5:
                s=int(data)
                lasts=s
            elif cmd == 'B' or cmd == 'b':
                bpm=int(data)
            elif cmd == 'Q' or cmd == 'q':
                ibi=int(data)
            t.append(t_now)  # 模擬數據增量流入,保存歷史數據
            m.append(lasts)  # 模擬數據增量流入,保存歷史數據
            if t_now > 200:
                del t[0]
                del m[0]
                #print(t[0],':',t[1],m[0],";",m[1])
                plt.clf()
            if t_now % 20 == 0:
                plt.plot(t, m, '-r')
                plt.title('BPM='+str(bpm))
                plt.legend(['IBI='+str(ibi)+'ms'], loc='upper left', framealpha=0.5,prop={'size':8})
            #plt.plot(t_now,s,'.r')
                plt.show()
                plt.pause(0.01)
            #print('S=', s, ' BPM=', bpm, ' IBI=', ibi, 'ms')
            t_now += 1

except KeyboardInterrupt:
    plt.close()
    ser.flushInput()
    ser.close()    # 清除序列通訊物件
    print('再見！')
    sys.exit()
except(OSError, serial.SerialException):
    print('Serial Port Error!!')
    sys.exit()