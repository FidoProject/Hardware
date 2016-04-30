import serial
import time

SERIAL_PORT = "/dev/tty.usbserial-AL029Z8F"
SERIAL_BAUD = 57600

s = serial.Serial(port=SERIAL_PORT, baudrate=SERIAL_BAUD)

def reset():
	moveJoint(1, 512);
	moveJoint(2, 800);
	moveJoint(3, 900);

	for i in range(0,5):
		gripper(i%2);
		time.sleep(0.5);

def getError(ID):
	return readDxl(ID,'e')

def readDxl(ID,cmd):
	#j m or s
	output = ""
	s.write('R'+str(cmd)+chr(ID))
	for line in s.readline():
		output = output + line
	return int(output)

def writeInt(inInt):
	inInt = int(inInt)
	s.write(chr(int(inInt)%256))
	s.write(chr(int(inInt)>>8))
	time.sleep(0.05)

def setTorque(torque):
	s.write('W'+'t'+chr(128))
	writeInt(torque)

def setSpeed(ID, speed):
	s.write('W'+'s'+chr(ID))
	writeInt(speed)

def moveJoint(ID, position):
	s.write('W'+'p'+chr(ID))
	writeInt(position)

def gripper(open):
	if (open):
		s.write('W'+'o'+chr(128))
		writeInt(500)
	else:
		s.write('W'+'c'+chr(128))
		writeInt(500)
