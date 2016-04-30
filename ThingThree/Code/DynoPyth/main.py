import serial
import time

from Dyno import *

def setupDxl():
	for i in range(1,6):
		init(i)

def testJoints():
    moveJoint(1,500,300)
    time.sleep(1)
    moveJoint(2,500,300)

setupDxl()
testJoints()
