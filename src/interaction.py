# -*- coding: UTF-8 -*-
import pika
import tuling
import os
import time

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel_TTS = connection.channel()
channel_ASR = connection.channel()

channel_TTS.queue_declare(queue='TTS')
channel_ASR.queue_declare(queue='ASR')


def callback(ch, method, properties, body):
    analyse(body)


def sendAnswer(text):
    channel_TTS.basic_publish(exchange='',
                              routing_key='TTS',
                              body=text)
    return


def analyse(body):
    connection.process_data_events()
    resp = tuling.chatbot(body)
    sendAnswer(resp['text'])


channel_ASR.basic_consume(callback, queue='ASR', no_ack=True)
channel_ASR.start_consuming()
