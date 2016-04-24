#!/usr/bin/python

import time
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
	return (int(r) << 16) + (int(g) << 8) + int(b)

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
		time.sleep(0.01);

	for i in range(0,numPixels/2):
		strip.setPixelColor(i, 0);
		strip.setPixelColor(numPixels-i, 0);
		strip.show();
		time.sleep(0.01);

pulseFromMiddle(0x00FF00)
