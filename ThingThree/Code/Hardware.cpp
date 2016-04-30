#include "Hardware.h"

Hardware::Hardware() {
    fd = serialOpen("/dev/ttyAMA0", 57600);
}

void Hardware::poise() {
	moveJoint(1, 512);
	moveJoint(2, 850);
	moveJoint(3, 950);
}

void Hardware::clap(int claps) {
	for (int i=0; i<1+claps*2; i++) {
		gripper(i%2);
		usleep(500000);
	}
}

void Hardware::getError(int id) {
	return readDxl(id,'e');
}

int Hardware::readDxl(int id, char cmd) {
	std::string val;
	while (serialDataAvailable(fd) > 0) val += serialGetchar(fd);

	return atoi(val.c_str());
}

void Hardware::writeInt(int inInt) {
	serialPutchar(fd,inInt%256);
	serialPutchar(fd,inInt>>8);
}

void Hardware::writeCommand(char readWrite, char command, int id) {
	std::string s = readWrite + command + ((char)id);
	serialPuts(fd,s.c_str());
}

void Hardware::setTorque(int id, int torque) {
	writeCommand('W','t',id);
	writeInt(torque);
}

void Hardware::setSpeed(int id, int speed) {
	writeCommand('W','s',id);
	writeInt(speed);
}

void Hardware::moveJoint(int id, int position) {
	writeCommand('W','p',id);
	writeInt(position);
}

void Hardware::gripper(bool open) {
	if (open) {
		writeCommand('W','o',128);
		writeInt(500);
	} else {
		writeCommand('W','c',128);
		writeInt(500);
	}
}

Hardware::~Hardware() {
	serialClose(fd);
}
