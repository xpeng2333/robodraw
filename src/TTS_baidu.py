#-*- coding:utf-8-*-
import pika
import ASR_TTS

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.queue_declare(queue='TTS')


def TTS_rabbit(text):
    api_key = "N0it8NCDVBt26v8Q9QCOaNzH"
    api_secret = "HR6Df4I07wNXbRdrHxf2uv93PCmAVdak"
    bdr = ASR_TTS.BaiduRest("test_python", api_key, api_secret)
    bdr.getVoice(text, "out.mp3")


def callback(ch, method, properties, body):
    TTS_rabbit(body)


channel.basic_consume(callback, queue='TTS', no_ack=True)

channel .start_consuming()
