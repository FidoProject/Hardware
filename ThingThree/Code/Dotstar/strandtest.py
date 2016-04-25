#!/usr/bin/python

import time, math
from dotstar import Adafruit_DotStar

numPixels = 24

dataPin   = 17
clockPin  = 27
strip     = Adafruit_DotStar(numPixels, dataPin, clockPin)

strip.begin()
strip.setBrightness(255)

def scale(color, brightness):
	str_hex = hex(color)[2:].zfill(6)
	r,g,b = (int(str_hex[2*x:2*x+2],16)*(brightness/255.0) for x in xrange(3))
	return (int(r) << 8) + (int(g) << 16) + int(b)

def pulseFade(color):
	for brightness in range(0,255):
		for i in range(0,numPixels):
			strip.setPixelColor(i, scale(color,brightness))
		strip.show()
		time.sleep(0.01)

	for brightness in range(255,0,-1):
		for i in range(0,numPixels):
			strip.setPixelColor(i, scale(color,brightness))
		strip.show()
		time.sleep(0.001)

def pulseFromMiddle(color):
	for i in range(0,numPixels/2):
		strip.setPixelColor(numPixels/2 + i, color);
		strip.setPixelColor(numPixels/2 - i, color);
		strip.show();
		time.sleep(0.02);

	for i in range(0,numPixels/2):
		strip.setPixelColor(i, 0);
		strip.setPixelColor(numPixels-i, 0);
		strip.show();
		time.sleep(0.02);

def cycle(color=-1):
	head = 0
	tail = -10

	curColor = 0xFF0000 if (color == -1) else color
	while True:
		strip.setPixelColor(head,curColor)
		strip.setPixelColor(tail,0)
		strip.show()
		time.sleep(0.02)

		head += 1
		if (head >= numPixels):
			head = 0
			if (color == -1):
				curColor >>= 8
				if (curColor == 0): curColor = 0xFF0000

		tail += 1
		if (tail >= numPixels): tail = 0

def breathe(color):
	while True:
		millis = int(round(time.time() * 1000))
		brightness = (math.exp(math.sin(millis/2000.0*math.pi)) - 0.36787944)*108.0;
		for i in range(0,numPixels):
			strip.setPixelColor(i, scale(color,brightness))
		strip.show()
                time.sleep(0.02)

cycle()
