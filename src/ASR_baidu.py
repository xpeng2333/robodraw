#-*-coding:utf-8-*-
import ASR_TTS
import pika
import json
import time
import numpy as np
from pyaudio import PyAudio, paInt16
import wave
import sys
import importlib
importlib.reload(sys)

save_count = 0
save_buffer = []
t = 0
sum = 0
time_flag = 0
flag_num = 0
filename = ''
duihua = '1'

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.queue_declare(queue='ASR')


def ASR_rabbit():
    bdr = ASR_TTS.BaiduRest("test_python")
    s = bdr.getText("input.wav")
    re_list = (json.loads(s).get('result'))
    if isinstance(re_list, list):
        list_text = re_list[0]
        print(list_text)
        channel.basic_publish(exchange='',
                              routing_key='ASR',
                              body=list_text)
        return


def save_wave_file(FILENAME, data):
    wf = wave.open(FILENAME, 'wb')
    wf.setnchannels(1)
    wf.setsampwidth(2)
    wf.setframerate(SAMPLING_RATE)
    wf.writeframes(b"".join(data))
    wf.close()

# channel.basic_consume(ASR_rabbit, queue='ASR', no_ack=True)

# channel.start_consuming()


'''
def callback(ch, method, properties, body):
    record()
'''
NUM_SAMPLES = 2048      # pyAudio内部缓存的块的大小
SAMPLING_RATE = 16000    # 取样频率
LEVEL = 1500            # 声音保存的阈值
COUNT_NUM = 10          # NUM_SAMPLES个取样之内出现COUNT_NUM个大于LEVEL的取样则记录声音
SAVE_LENGTH = 8         # 声音记录的最小长度：SAVE_LENGTH * NUM_SAMPLES 个取样


pa = PyAudio()
stream = pa.open(format=paInt16, channels=1, rate=SAMPLING_RATE, input=True,
                 frames_per_buffer=NUM_SAMPLES)


while True:
    # 读入NUM_SAMPLES个取样
    string_audio_data = stream.read(NUM_SAMPLES)
    # 将读入的数据转换为数组
    audio_data = np.fromstring(string_audio_data, dtype=np.short)
    # 计算大于LEVEL的取样的个数
    large_sample_count = np.sum(audio_data > LEVEL)

    temp = np.max(audio_data)
    if temp > 2000 and t == 0:
        t = 1  # 开启录音
        print("检测到信号，开始录音,计时五秒")
        begin = time.time()
    if t:
        if np.max(audio_data) < 1000:
            sum += 1
        end = time.time()
        if end - begin > 5:
            time_flag = 1
            print("五秒到了，准备结束")
        # 如果个数大于COUNT_NUM，则至少保存SAVE_LENGTH个块
        if large_sample_count > COUNT_NUM:
            save_count = SAVE_LENGTH
        else:
            save_count -= 1

        if save_count < 0:
            save_count = 0

        if save_count > 0:
            # 将要保存的数据存放到save_buffer中
            save_buffer.append(string_audio_data)
        else:
            # 将save_buffer中的数据写入WAV文件，WAV文件的文件名是保存的时刻
            # if  time_flag:
            if len(save_buffer) > 0 or time_flag:
                # filename = datetime.now().strftime("%Y-%m-%d_%H_%M_%S") + ".wav"#原本是用时间做名字
                filename = "input.wav"
                flag_num += 1

                save_wave_file(filename, save_buffer)
                save_buffer = []
                t = 0
                sum = 0
                time_flag = 0
                print(filename, "保存成功正在进行语音识别")
                ASR_rabbit()
