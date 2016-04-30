#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cmath>

#include <wiringSerial.h>

class Hardware {
public:
    Hardware();

	void inverseKinematicsXY(double x, double y, double *theta1, double *theta2);

	// SERVOS
	void poise();
	void clap(int claps);
	int getError(int id);
	void setJoints(double i, double j, double k);
	void setTorque(int id, int torque);
	void setSpeed(int id, int speed);
	void gripper(bool open);

    virtual ~Hardware();
private:
	void writeCommand(char readWrite, char command, int id);
	void writeInt(int inInt);
	int readDxl(int id, char cmd);

	double map(double x, double in_min, double in_max, double out_min, double out_max);

	void scaleServos(double iAng, double jAng, double kAng, int *iVal, int *jVal, int *kVal);

	void moveJoint(int id, int position);

	int fd;
};

#endif /* HARDWARE_H_ */
