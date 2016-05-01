#define START_1 512
#define START_2 880
#define START_3 950
#define GRIPPER_OPEN 800
#define GRIPPER_CLOSED 500

#define SONAR_TRIGGER 3
#define SONAR_LEFT 2
#define SONAR_RIGHT 1

Dynamixel Dxl(1);

void gripper(boolean open) {
	if (open) {
		Dxl.goalPosition(4,GRIPPER_OPEN);
		Dxl.goalPosition(5,1024-GRIPPER_OPEN);
	} else {
		Dxl.goalPosition(4,GRIPPER_CLOSED);
		Dxl.goalPosition(5,1024-GRIPPER_CLOSED);		
	}
}

void setup() {
	Serial2.begin(57600);
	Dxl.begin(3);

	pinMode(SONAR_LEFT, INPUT_ANALOG);
	pinMode(SONAR_RIGHT, INPUT_ANALOG);
	pinMode(SONAR_TRIGGER, OUTPUT);

	digitalWrite(SONAR_TRIGGER, LOW);

	for (int id=1; id<6; id++) {
		Dxl.jointMode(id);
		Dxl.ledOn(id);
	} 

	Dxl.writeWord(BROADCAST_ID,11,100);

	Dxl.goalPosition(1, START_1);
	Dxl.goalPosition(2, START_2);
	Dxl.goalPosition(3, START_3);
	
	for (int i=0; i<5; i++) {
		gripper(i%2);
		delay(500);
	}
}

byte blockingRead() {
	while (!Serial2.available()) delay(10);
	return Serial2.read();
}

boolean readInt(int *outVal) {
	byte inBuffer[sizeof(short)];
	

	for (int i=0; i<sizeof(short); i++) {
		inBuffer[i] = blockingRead();
	}
	
	*outVal = *((short*)inBuffer);	
	return true; 
}

void processWrite(byte cmd, byte id) {
	int val;
	readInt(&val);

  	switch (cmd) { 
		case 's': { // set speed
			Dxl.goalSpeed(id, val);
			break;
		} case 'p': { // move to position
			Dxl.goalPosition(id, val);
			break; 
		} case 'o': {
			gripper(true);
			break;
		} case 'c': {
			gripper(false);
			break;
		} case 't': {
			Dxl.maxTorque(id,val);
			break;
		} case 'g': { // general write command
			int add; 
			if (!readInt(&add)) break; 
			Dxl.writeWord(id, add, val);
			break;
		
			// examples of using writeWord()
			//Dxl.writeWord(id, 30, val); //move to Goal Position
			//Dxl.writeWord(id, 32, val); //move at Speed
			//Dxl.writeWord(id, 8, 0); //set wheel mode
		}
	}
}

void processRead(byte cmd, byte id) {
	/* gets sent some 'complex' command in the cmd byte and then executes */
	switch (cmd) {
		case 'j': { // read current joint position
			char str[4]; // create an array of characters
			int output = Dxl.readWord(id, 36); // query the dynamixel
			itoa(output, str, 10); // turns integer to ascii
			// return Position. Values between 0-999
			Serial2.write(str);
			break;
		} case 's': { // read current speed
			char str[4]; // create an array of characters
			int output = Dxl.readWord(id, 38); // query the dynamixel
			itoa(output, str, 10); // turns integer to ascii
  			Serial2.write(str);
			break;
		} case 'e': { // read error
			int output = Dxl.readWord(id, 18); // query the dynamixel
  			Serial2.println(output);
			break;
		} case 'v': {
			digitalWrite(SONAR_TRIGGER, HIGH);
  			delay(10);
  			
  			int sonarOne = analogRead(SONAR_LEFT);
    			int sonarTwo = analogRead(SONAR_RIGHT);
  
  			SerialUSB.println(sonarOne);
  			SerialUSB.println(sonarTwo);
			break;
		}
	}
}

void loop() {
	// wait for a start byte, 'W' for write or 'R' for read
	while (Serial2.available()) {
		byte inByte = blockingRead();
		byte cmd = blockingRead();
		byte id = blockingRead();

		if (inByte == 'W') processWrite(cmd, id);
		else processRead(cmd, id);
	}
}


