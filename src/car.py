# -*- coding: UTF-8 -*-

import threading
import RPi.GPIO as GPIO
import time


class Info:
    @staticmethod
    def p(message):
        print 'Info: ' + message

# Wheel封装的单个车轮的所有可能操作


class Wheel:
    pins = {'l': [13, 15], 'r': [16, 18]}

    def __init__(self, name):
        self.name = name
        self.pin = Wheel.pins[self.name]
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(self.pin[0], GPIO.OUT)
        GPIO.setup(self.pin[1], GPIO.OUT)
        self.stop()

    def st(self):
        print 'ss'

    def forward(self):
        Info.p('wheel ' + self.name + ' is forwarding')
        GPIO.output(self.pin[0], GPIO.HIGH)
        GPIO.output(self.pin[1], GPIO.LOW)

    def stop(self):
        GPIO.output(self.pin[0], GPIO.LOW)
        GPIO.output(self.pin[1], GPIO.LOW)

    def back(self):
        Info.p('wheel ' + self.name + ' is backing')
        GPIO.output(self.pin[0], GPIO.LOW)
        GPIO.output(self.pin[1], GPIO.HIGH)

# Car组合四个轮子所有可能操作


class Car:
    wheel = [Wheel('l'), Wheel('r')]

    @staticmethod
    def init():
        GPIO.setmode(GPIO.BOARD)
        Info.p('initialize the car ....')
        Info.p('Car is ready to fly!')

    @staticmethod
    def forward():
        Info.p('go straight forward')
        for wheel in Car.wheel:
            wheel.forward()

    @staticmethod
    def left():
        Info.p('turn left ')
        Car.wheel[0].back()
        Car.wheel[1].forward()

    @staticmethod
    def right():
        Info.p('turn right ')
        Car.wheel[0].forward()
        Car.wheel[1].back()

    @staticmethod
    def stop():
        Info.p('stop ')
        for wheel in Car.wheel:
            wheel.stop()

    @staticmethod
    def back():
        Info.p('go straight back')
        for wheel in Car.wheel:
            wheel.back()

    @staticmethod
    def lforward():
        Info.p('go forward and turn left')
        Car.wheel[1].forward()

    @staticmethod
    def rforward():
        Info.p('go forward and turn right')
        Car.wheel[0].forward()

    @staticmethod
    def lback():
        Info.p('go back and turn left')
        Car.wheel[1].back()

    @staticmethod
    def rback():
        Info.p('go back and turn right')
        Car.wheel[0].back()

    @staticmethod
    def stop():
        Info.p('try to stop the car ...')
        for wheel in Car.wheel:
            wheel.stop()
