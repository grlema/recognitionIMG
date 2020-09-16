# Heart Rate Test

import argparse
import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import detrend

helpText = "'Esc' to Quit"


def parse_args():
    parser = argparse.ArgumentParser(description="CV2 Video Capture PhotoPlethysmoGram Test")
    parser.add_argument("--vid", dest="video_dev",
                        help="video device # of USB webcam (/dev/video?) [1]",
                        default=0, type=int)
    args_ret = parser.parse_args()
    return args_ret


def line(x0, x1, y0, y1, func):
    isSwap = abs(y1 - y0) > abs(x1 - x0)
    if isSwap:
        x0, y0 = y0, x0
        x1, y1 = y1, x1
    if x0 > x1:
        x0, x1 = x1, x0
        y0, y1 = y1, y0
    deltax = x1 - x0
    deltay = abs(y1 - y0)
    error = deltax / 2
    y = y0
    if y0 < y1:
        ystep = 1
    else:
        ystep = -1
    for x in range(x0, x1):
        if isSwap:
            func(y, x)
        else:
            func(x, y)
        error = error - deltay
        if error < 0:
            y = y + ystep
            error = error + deltax


def plot_r(x, y):
    frame[int(y), int(x)] = [255, 0, 0]


def plot_g(x, y):
    frame[int(y), int(x)] = [0, 255, 0]


def plot_b(x, y):
    frame[int(y), int(x)] = [0, 0, 255]


def plot_w(x, y):
    frame[int(y), int(x)] = [255, 255, 255]


DATA_LENGTH = 100 #64
if __name__ == "__main__":
    # plot init
    xx = np.arange(10000)
    yy = np.random.randn(10000)
    fig, ax = plt.subplots(4, figsize=(4, 8), linewidth=0.1)
    fig.subplots_adjust(hspace=0.5)

    # arg parsing
    args = parse_args()
    print("Called with args:")
    print(args)
    # open video device
    cap = cv2.VideoCapture(args.video_dev)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640) #300
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480) #300

    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    px = int(width / 2)  # float
    py = int(height / 2)  # float
    print(py, px)

    drawDataList = []
    red_filtered = []
    # plots setup
    li1, = ax[0].plot(xx, yy, color='r')
    ax[0].set_xlim(0, DATA_LENGTH)
    ax[0].set_ylim(-255, 255)
    li2, = ax[1].plot(xx, yy, color='g')
    ax[1].set_xlim(0, DATA_LENGTH)
    ax[1].set_ylim(-255, 255)
    li3, = ax[2].plot(xx, yy, color='b')
    ax[2].set_xlim(0, DATA_LENGTH)
    ax[2].set_ylim(-255, 255)

    li4, = ax[3].plot(xx, yy,)
    ax[3].set_xlim(0, DATA_LENGTH)
    ax[3].set_ylim(-255, 255)


    while (True):
        ret, frame = cap.read()
        if ret:
            cv2.putText(frame, helpText, (11, 20), cv2.FONT_HERSHEY_PLAIN, 1.0, (32, 32, 32), 4, cv2.LINE_AA)
            cv2.putText(frame, helpText, (10, 20), cv2.FONT_HERSHEY_PLAIN, 1.0, (240, 240, 240), 1, cv2.LINE_AA)

            b, g, r = cv2.split(frame)

            avg_color = [np.average(b), np.average(g), np.average(r)]
            #print(np.average(r))

            drawDataList.append(avg_color)
            if len(drawDataList) > DATA_LENGTH:
                drawDataList.pop(0)
            result = np.reshape(drawDataList, (len(drawDataList), 3))

            red = 255 - result[:, 2]
            red_filtered = red - np.average(red[-32:])
            #
            green = 255 - result[:, 1]
            green_filtered = green - np.average(green[-32:])
            #
            blue = 255 - result[:, 0]
            blue_filtered = blue - np.average(blue[-32:])

            #test mixer r+g+b
            all_filtered = red_filtered + green_filtered + blue_filtered
            li4.set_xdata(np.arange(len(all_filtered)))
            li4.set_ydata(all_filtered * 100)

            # print(red_filtered.dtype)
            li1.set_xdata(np.arange(len(red_filtered)))
            li1.set_ydata(red_filtered * 100)
            li2.set_xdata(np.arange(len(green_filtered)))
            li2.set_ydata(green_filtered * 100)
            li3.set_xdata(np.arange(len(blue_filtered)))
            li3.set_ydata(blue_filtered * 100)
            #
            now_x = 0
            old_x = 0
            old_ry = 0
            old_gy = 0
            old_by = 0
            for p in drawDataList:
                by = int((255 - p[0]) * 1.0)
                gy = int((255 - p[1]) * 1.0)
                ry = int((255 - p[2]) * 1.0)

                if now_x == 0:
                    frame[old_by, now_x] = [255, 0, 0]
                    frame[old_gy, now_x] = [0, 255, 0]
                    frame[old_ry, now_x] = [0, 0, 255]
                else:
                    line(old_x, now_x, old_by, by, plot_r)
                    line(old_x, now_x, old_gy, gy, plot_g)
                    line(old_x, now_x, old_ry, ry, plot_b)
                    line(old_x + 1, now_x + 1, old_ry + 1, ry + 1, plot_b)
                    line(old_x + 2, now_x + 2, old_ry + 2, ry + 2, plot_w)
                    # dx = now_x - old_x
                    ##dx = 1
                    # dby = by - old_by
                    # dgy = gy - old_gy
                    # dry = ry - old_ry

                    # for y in range(old_by, by, 1 if old_by < by else -1):
                    #    x = old_x + (dx * (y - old_by)) / dby
                    #    frame[int(y), int(x)] = [255,0,0]
                    # for y in range(old_gy, gy, 1 if old_gy < gy else -1):
                    #    x = old_x + (dx * (y - old_gy)) / dgy
                    #    frame[int(y), int(x)] = [0,255,0]
                    # for y in range(old_ry, ry, 1 if old_ry < ry else -1):
                    #    x = old_x + (dx * (y - old_ry)) / dry
                    #    frame[int(y), int(x)] = [0,0,255]

                old_x = now_x
                old_by = by
                old_gy = gy
                old_ry = ry
                now_x = now_x + 2

            cv2.imshow('frame', frame)
            if cv2.waitKey(1) & 0xFF == 27:
                break
            plt.pause(0.001)

    cap.release()
    cv2.destroyAllWindows()
    plt.close()