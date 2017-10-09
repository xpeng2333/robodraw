# -*- coding: UTF-8 -*-

import socket

SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 建立连接:
s.connect(('192.168.43.101', 9999))
# 接收欢迎消息:
print(s.recv(SIZE))
s.send('begin to send')
print('sending, please wait for a second ...')
with open('../data/text/out_step.txt', 'rb') as f:
    for data in f:
        s.send(data)
print('sended !')
s.close()
print('connection closed')
