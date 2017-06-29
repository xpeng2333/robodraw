# coding:UTF-8
import requests
print("你好，我是图灵机器人")
while 1:
    s = input()
    resp = requests.post("http://www.tuling123.com/openapi/api", data={
        #        "key": "d59c41e816154441ace453269ea08dba",
        "key": "ff772ad12e0c421f98da2dd7f6a9289c",
        "info": s,
        "userid": "123456"
    })
    resp = resp.json()
    print(resp['text'])
