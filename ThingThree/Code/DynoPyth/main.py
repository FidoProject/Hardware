import serial
import time

from Dyno import *

while True:
	gripper(True)
	time.sleep(0.5)
	gripper(False)
	time.sleep(0.5)
