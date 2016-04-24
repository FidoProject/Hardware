#!/usr/bin/python

import time
from dotstar import Adafruit_DotStar

numpixels = 24

datapin   = 17
clockpin  = 27
strip     = Adafruit_DotStar(numpixels, datapin, clockpin)

strip.begin()
strip.setBrightness(255)

for color in range(0,255):
	for i in range(0,numpixels):
		strip.setPixelColor(i, color << 16)
	strip.show()
	time.sleep(0.01)

for color in range(255,0,-1):
	for i in range(0,numpixels):
		strip.setPixelColor(i, color << 16)
	strip.show()
	time.sleep(0.001)
