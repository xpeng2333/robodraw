# -*- coding: UTF-8 -*-
import serial
from time import sleep
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.5)
fpath = open("out.txt")
path_list = []
serdata = ''
for m in fpath.readline():
    if m != '@':
        serdata = serdata + m
    else:
        path_list.append(serdata)
        serdata = ''

i = 3
ser.write(bytes(path_list[0], encoding="utf8"))
ser.write(bytes(path_list[1], encoding="utf8"))
ser.write(bytes(path_list[2], encoding="utf8"))

while True:
    if (ser.read() == b'@'):
        ser.write(bytes(path_list[i], encoding="utf8"))
        ser.flushInput()
        i = i + 1
