#include "Hardware.h"

#define LENGTH_ONE 10.0
#define LENGTH_TWO 10.0

#define I_MIN_ANG -90
#define I_MAX_ANG 90
#define I_MIN_VAL 0
#define I_MAX_VAL 1023

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

int Hardware::getError(int id) {
	return readDxl(id,'e');
}

int Hardware::readDxl(int id, char cmd) {
	std::string val;
	while (serialDataAvail(fd) > 0) val += serialGetchar(fd);

	return atoi(val.c_str());
}

void Hardware::writeInt(int inInt) {
	serialPutchar(fd,inInt%256);
	serialPutchar(fd,inInt>>8);
}

void Hardware::writeCommand(char readWrite, char command, int id) {
	std::string s;
	s += readWrite;
	s += command;
	s += static_cast<char>(id);
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


void Hardware::gripper(bool open) {
	if (open) {
		writeCommand('W','o',128);
		writeInt(500);
	} else {
		writeCommand('W','c',128);
		writeInt(500);
	}
}

void Hardware::setJoints(double i, double j, double k) {
	int iVal, jVal, kVal;
	scaleServos(i, j, k, &iVal, &jVal, &kVal);
	moveJoint(1,iVal);
	//moveJoint(2,jVal);
	//moveJoint(3,kVal);
}

void Hardware::moveJoint(int id, int position) {
	writeCommand('W','p',id);
	writeInt(position);
}

double Hardware::map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Hardware::scaleServos(double iAng, double jAng, double kAng, int *iVal, int *jVal, int *kVal) {
	iVal = round(map(iAng,I_MIN_ANG,I_MAX_ANG,I_MIN_VAL,I_MAX_VAL));
}

void Hardware::inverseKinematicsXY(double x, double y, double *theta1, double *theta2) {
	double hypo = sqrt(pow(x,2) + pow(y,2));
	double q1 = atan2(y,x);
	double q2 = acos((pow(LENGTH_ONE,2) - pow(LENGTH_TWO,2) + pow(hypo,2))
					 /(2.0*LENGTH_ONE*hypo));

	theta1 = q1 + q2;
	theta2 = acos((pow(LENGTH_ONE,2) + pow(LENGTH_TWO,2) - pow(hypo,2))
					 /(2.0*LENGTH_ONE*LENGTH_TWO));
}

Hardware::~Hardware() {
	serialClose(fd);
}
