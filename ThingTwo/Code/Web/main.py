#!/usr/bin/python

from flask import Flask, request
import RPi.GPIO as GPIO

app = Flask(__name__, static_url_path='')

GPIO.setmode(GPIO.BCM)

GPIO.setup(6, GPIO.OUT)
m1 = GPIO.PWM(6,50)
GPIO.setup(16, GPIO.OUT)

GPIO.setup(19, GPIO.OUT)
m2 = GPIO.PWM(19,50)
GPIO.setup(20, GPIO.OUT)

GPIO.setup(26, GPIO.OUT)
m3 = GPIO.PWM(26,50)
GPIO.setup(21, GPIO.OUT)


@app.route('/')
def index():
    return app.send_static_file('index.html')

@app.route('/drive/<int:i>/<int:j>/<int:k>', methods=['GET'])
def drive(i, j, k):
    GPIO.output(16,i>0)
    m1.ChangeDutyCycle(abs(i))

    GPIO.output(20,i>0)
    m2.ChangeDutyCycle(abs(j))

    GPIO.output(21,i<0)
    m3.ChangeDutyCycle(abs(k))


if __name__ == '__main__':
    m1.start(0)
    m2.start(0)
    m3.start(0)

    app.run(host= '0.0.0.0',port=80,debug=True)
