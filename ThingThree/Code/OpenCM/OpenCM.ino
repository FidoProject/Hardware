Dynamixel Dxl(1);

void setup() {
	Serial2.begin(57600);
	Dxl.begin(3);
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
	switch (cmd) { 
		case 'i': { // initialize
			Dxl.jointMode(id); 
			break; 
		} case 'n': { // set new id
			int newID;
			if (!readInt(&newID)) break;
			Dxl.setID(id, newID);
			Dxl.jointMode(newID);
			break;
		} case 's': { // set speed
			int add, val;
			if (!(readInt(&add) && readInt(&val))) break; 
			//Dxl.writeWord(id, add, val);
			Dxl.goalSpeed(id, val);
			break;
		} case 'p': { // move to position
			int pos, vel; 
			if (!(readInt(&pos) && readInt(&vel))) break; 
			Dxl.setPosition(id, pos, vel);
			break; 
		} case 'g': { // general write command
			int add, val; 
			if (!(readInt(&add) && readInt(&val))) break; 
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
		}
	}
}

void loop() {
	// wait for a start byte, 'W' for write or 'R' for read
	while (Serial2.available()) {
		byte inByte = blockingRead();
		byte cmd = blockingRead();
		byte id = blockingRead();

		(inByte == 'W') ? processWrite(cmd, id) : processRead(cmd, id);
	}		
}


