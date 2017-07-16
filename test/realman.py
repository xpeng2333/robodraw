# -*- coding: utf-8 -*-
import requests
import json
import base64
import wave
from pydub import AudioSegment  # 需要安装pydub、ffmpeg
import io
import sys
import os
from pyaudio import PyAudio, paInt16
import numpy as np
from datetime import datetime
import time


class BaiduRest:
    def __init__(self, cu_id, api_key, api_secert):
        # token认证的url
        self.token_url = "https://openapi.baidu.com/oauth/2.0/token"
        # 语音合成的resturl
        self.getvoice_url = "http://tsn.baidu.com/text2audio"
        # 语音识别的resturl
        self.upvoice_url = 'http://vop.baidu.com/server_api'
        self.cu_id = cu_id
        self.getToken(api_key, api_secert)
        return

    def getToken(self, api_key, api_secert):
        # 1.获取token
        data = {'grant_type': 'client_credentials',
                'client_id': api_key, 'client_secret': api_secert}
        r = requests.post(self.token_url, data=data)
        Token = json.loads(r.text)
        self.token_str = Token['access_token']

    def getVoice(self, text, filename):
        # 2. 向Rest接口提交数据
        data = {'tex': text, 'lan': 'zh', 'cuid': self.cu_id,
                'ctp': 1, 'tok': self.token_str}
        r = requests.post(self.getvoice_url, data=data, stream=True)
        voice_fp = open(filename, 'wb')
        voice_fp.write(r.raw.read())
        # for chunk in r.iter_content(chunk_size=1024):
        # voice_fp.write(chunk)
        voice_fp.close()

    def getText(self, filename):
        # 2. 向Rest接口提交数据
        data = {"format": "wav", "rate": 16000, "channel": 1,
                "token": self.token_str, "cuid": self.cu_id, "lan": "zh"}
        # 语音的一些参数
        wav_fp = open(filename, 'rb')
        voice_data = wav_fp.read()
        data['len'] = len(voice_data)
        data['speech'] = base64.b64encode(voice_data).decode('utf-8')
        post_data = json.dumps(data)
        r = requests.post(self.upvoice_url, data=bytes(
            post_data, encoding="utf-8"))
        # 3.处理返回数据
        return r.text

    def ConvertToWav(self, filename, wavfilename):
        # 先从本地获取mp3的bytestring作为数据样本
        fp = open("out.mp3", 'rb')
        data = fp.read()
        fp.close()
        # 主要部分
        aud = io.BytesIO(data)
        sound = AudioSegment.from_file(aud, format='mp3')
        raw_data = sound._data
        # 写入到文件，验证结果是否正确。
        l = len(raw_data)
        f = wave.open(wavfilename, 'wb')
        f.setnchannels(1)
        f.setsampwidth(2)
        f.setframerate(16000)
        f.setnframes(l)
        f.writeframes(raw_data)
        f.close()
        return wavfilename


if __name__ == "__main__":
    # api_key和api_secert 自行编写
    api_key = "N0it8NCDVBt26v8Q9QCOaNzH"
    api_secert = "HR6Df4I07wNXbRdrHxf2uv93PCmAVdak"
    # 初始化
    bdr = BaiduRest("test_python", api_key, api_secert)
    # 将字符串语音合成并保存为out.mp3
    bdr.getVoice(
        "大家好，我是怪咖达芬奇队的小芬奇！", "out.mp3")
    # 识别test.wav语音内容并显示
    # print(bdr.getText(bdr.ConvertToWav("out.mp3", "test.wav")))

    os.system("mpg123 out.mp3")

NUM_SAMPLES = 2048      # pyAudio内部缓存的块的大小
SAMPLING_RATE = 16000    # 取样频率
LEVEL = 1500            # 声音保存的阈值
COUNT_NUM = 20          # NUM_SAMPLES个取样之内出现COUNT_NUM个大于LEVEL的取样则记录声音
SAVE_LENGTH = 8         # 声音记录的最小长度：SAVE_LENGTH * NUM_SAMPLES 个取样

# 开启声音输入ｐｙａｕｄｉｏ对象
pa = PyAudio()
stream = pa.open(format=paInt16, channels=1, rate=SAMPLING_RATE, input=True,
                 frames_per_buffer=NUM_SAMPLES)

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