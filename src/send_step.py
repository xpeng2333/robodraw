# -*- coding: utf-8 -*-
import serial
from time import sleep
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5)
fpath = open("./out_step.txt")
step_list = []
serdata = ''

num = 0.0
for m in fpath.readline():
    if m != '@':
        serdata = serdata + m
    else:
        step_list.append(serdata)
        num += 1
        serdata = ''

i = 3
ser.write(bytes(step_list[0], encoding="utf8"))
ser.write(bytes(step_list[1], encoding="utf8"))
ser.write(bytes(step_list[2], encoding="utf8"))

while True:
    if (ser.read() == b'@'):
        ser.flushInput()
        try:
            ser.write(bytes(step_list[i], encoding="utf8"))
            i = i + 1
            if(i % 100 == 0):
                p = 100.0 * i / num
                print(p)
        except:
            print(i)
            print(step_list[i - 1:i + 1])
            break
