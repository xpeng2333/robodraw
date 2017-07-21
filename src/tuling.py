# coding:utf-8
import requests


def chatbot(body):
    resp = requests.post("http://www.tuling123.com/openapi/api", data={
        #        "key": "d59c41e816154441ace453269ea08dba",
        "key": "ff772ad12e0c421f98da2dd7f6a9289c",
        "info": body,
        "userid": "xiaopeng"
    })
    resp = resp.json()
    return resp
