# -*- coding: UTF-8 -*-
import serial
from time import sleep
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.5)
fpath = open("../data/text/circle1.txt")
path_list = []
# ser.write(b'$1#200#1#100#1#150#')
#-446	647	-864
# 25	5	10
serdata = ''
for m in fpath.readline():
    if m != '@':
        serdata = serdata + m
    else:
        path_list.append(serdata)
        serdata = ''
#     ser.write(Byte(serdata)
 #   print(serdata)
'''
sleep(1000)
path=[b'$1#200#1#100#1#150#', b'$0#200#0#100#0#150#']
path.append(b'$0#200#0#100#0#150#')
i=0
'''
i = 1
ser.write(bytes(path_list[0], encoding="utf8"))
while True:
 #   sleep(0.2)
    # print(str(ser.read()))
    if (ser.read() == b'@'):
        ser.write(bytes(path_list[i], encoding="utf8"))
        ser.flushInput()
        i = i + 1
