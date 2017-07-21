# coding:UTF-8
import pika
import ASR_TTS

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel_TTS = connection.channel()
channel_ASR = connection.channel()

channel_TTS.queue_declare(queue='TTS')
channel_ASR.queue_declare(queue='ASR')

