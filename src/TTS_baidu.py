#-*- coding:utf-8-*-
import pika
import ASR_TTS

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.queue_declare(queue='TTS')


def TTS_rabbit(text):
    bdr = ASR_TTS.BaiduRest("test_python")
    bdr.getVoice(text, "../data/audio/output.mp3")


def callback(ch, method, properties, body):
    TTS_rabbit(body)


channel.basic_consume(callback, queue='TTS', no_ack=True)

channel.start_consuming()
