#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cmath>
#include <wiringSerial.h>

#include "Hardware.h"

#define LENGTH_ONE 11.5
#define LENGTH_TWO 16.0

#define I_MIN_VAL 0
#define I_MAX_VAL 1023

#define J_MIN_VAL 230
#define J_MAX_VAL 850

#define K_MIN_VAL 950
#define K_MAX_VAL 20

#define SPHERE_CENTER_Z -5
#define SPHERE_RADIUS 7

Hardware::Hardware() {
    fd = serialOpen("/dev/ttyAMA0", 57600);
}

void Hardware::good() {
	system("sudo python ~/Hardware/ThingThree/Code/Dotstar/strandtest.py 0xFF0000 1 &");
}

void Hardware::bad() {
        system("sudo python ~/Hardware/ThingThree/Code/Dotstar/strandtest.py 0x00FF00  1 &");
}

void Hardware::neutral() {
	system("sudo python ~/Hardware/ThingThree/Code/Dotstar/strandtest.py 0x0000FF 1 &");
}

void Hardware::poise() {
	moveJoint(1, 512);
	moveJoint(2, 860);
	moveJoint(3, 960);
}

void Hardware::clap(int claps) {
	for (int i=0; i<1+claps*2; i++) {
		gripper(i%2);
		usleep(500000);
	}
}

int Hardware::getServoError(int id) {
	return readDxl(id,'e');
}

void Hardware::getSonars(int *sonarOne, int *sonarTwo) {
	writeCommand('R','v',128);

	usleep(100000);

	std::string val;
	while (serialDataAvail(fd) > 0) val += serialGetchar(fd);

	std::string one = val.substr(0, val.find("\n"));
	std::string two = val.substr(val.find("\n"));

	*sonarOne = atoi(one.c_str());
	*sonarTwo = atoi(two.c_str());
}

int Hardware::readDxl(int id, char cmd) {
	writeCommand('R',cmd,id);

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

bool Hardware::setJoints(double i, double j, double k) {
	double y, z;
	forwardKinematicsXY(j, k, &y, &z);

	double x = -y*sin(i*0.0174532925);
	y *= cos(i*0.0174532925);

	if (!safetyCheck(x,y,z)) return false;

	int iVal, jVal, kVal;
	scaleServos(i, j, k, &iVal, &jVal, &kVal);
	moveJoint(1,iVal);
	moveJoint(2,jVal);
	moveJoint(3,kVal);

	return true;
}

bool Hardware::safetyCheck(double x, double y, double z) {
	double radius = sqrt(x*x + y*y + pow(z-SPHERE_CENTER_Z,2));
	if (radius < SPHERE_RADIUS) return false;
	else if (z < -5) return false;
	else return true;
}

bool Hardware::setEffectorPosition(double theta, double x, double y) {
	double theta1, theta2;
	inverseKinematicsXY(x, y, &theta1, &theta2);

	return setJoints(theta,theta1,theta2);
}

void Hardware::moveJoint(int id, int position) {
	writeCommand('W','p',id);
	writeInt(position);
}

void Hardware::scaleServos(double iAng, double jAng, double kAng, int *iVal, int *jVal, int *kVal) {
	*iVal = round(map(iAng,I_MIN_ANG,I_MAX_ANG,I_MIN_VAL,I_MAX_VAL));
	*jVal = round(map(jAng,J_MIN_ANG,J_MAX_ANG,J_MIN_VAL,J_MAX_VAL));
	*kVal = round(map(kAng,K_MIN_ANG,K_MAX_ANG,K_MIN_VAL,K_MAX_VAL));
}

void Hardware::forwardKinematicsXY(double theta1, double theta2, double *x, double *y) {
	double theta3 = theta2 - (180 - theta1);
	*x = LENGTH_ONE*cos(theta1*0.0174533) + LENGTH_TWO*cos(theta3*0.0174533);
	*y = LENGTH_ONE*sin(theta1*0.0174533) + LENGTH_TWO*sin(theta3*0.0174533);
}

void Hardware::inverseKinematicsXY(double x, double y, double *theta1, double *theta2) {
	double hypo = sqrt(pow(x,2) + pow(y,2));
	double q1 = atan2(y,x);
	double q2 = acos((pow(LENGTH_ONE,2) - pow(LENGTH_TWO,2) + pow(hypo,2))
					/(2.0*LENGTH_ONE*hypo));

	*theta1 = q1 + q2;
	*theta2 = acos((pow(LENGTH_ONE,2) + pow(LENGTH_TWO,2) - pow(hypo,2))
				  /(2.0*LENGTH_ONE*LENGTH_TWO));

	*theta1 *= 180.0/3.14159265;
	*theta2 *= 180.0/3.14159265;
}

Hardware::~Hardware() {
	serialClose(fd);
}
