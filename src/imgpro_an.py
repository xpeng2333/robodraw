# coding:UTF-8
import pika

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel_TTS = connection.channel()
channel_ASR = connection.channel()

channel_TTS.queue_declare(queue='TTS')
channel_ASR.queue_declare(queue='ASR')


def publish_text(text):
    channel_TTS.basic_publish(exchange='',
                              routing_key='TTS',
                              body=text)


def receive_text(ch, method, properties, body):
    pass


channel_ASR.basic_consume(receive_text, queue='ASR', no_ack=True)
channel_ASR.start_consuming()
