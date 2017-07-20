#-*-coding:utf-8-*-
import ASR_TTS
import pika
import json

connection = pika.BlockingConnection(
    pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

channel.queue_declare(queue='ASR')


def ASR_rabbit():
    h = input()
    bdr = ASR_TTS.BaiduRest("test_python")
    s = bdr.getText("input.wav")
    list_text = (json.loads(s).get('result'))[0]
    print(list_text)
    channel.basic_publish(exchange='',
                          routing_key='ASR',
                          body=list_text)


def callback(ch, method, properties, body):
    ASR_rabbit()


#channel.basic_consume(ASR_rabbit, queue='ASR', no_ack=True)

# channel.start_consuming()
def main():
    ASR_rabbit()


if __name__ == '__main__':
    main()
