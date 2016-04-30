import serial
import time

s = serial.Serial()
s.baudrate = 57600                        # baud rate, in bits/second
s.port = "/dev/ttyAMA0"            # this is whatever port your are using
s.timeout = 3.0
s.open()



def init(ID):
        s.write('W'+'i'+chr(ID))

def readDxl(ID,cmd):
        #j m or s
        output = ""
        s.write('R'+str(cmd)+chr(ID))
        for line in s.readline():
                output = output + line
        return int(output[-3:])        

def moveJoint(ID, position, speed):
        #move to position between 0-1024
        #512 is 12 o'clock
        #WRITE ID
        s.write('W'+'p'+chr(ID))

        #write position        
        position = int(position)
        s.write(chr(int(position)%256))
        s.write(chr(int(position)>>8))
        
        #WRITE SPEED
        velocity = int(speed)
        s.write(chr(int(velocity)%256))
        s.write(chr(int(velocity)>>8))

        #READ POSITION
        time.sleep(0.5)
        print(readDxl(ID,"j"))
