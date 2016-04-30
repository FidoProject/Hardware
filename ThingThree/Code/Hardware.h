#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <string>
#include <wiringSerial.h>

class Hardware {
public:
    Hardware();

	// SERVOS
	void poise();
	void clap(int claps);
	void getError(int id);
	void setTorque(int torque);
	void setSpeed(int id, int speed);
	void moveJoint(int id, int position);
	void gripper(bool open);

    virtual ~Hardware();
private:
	void Hardware::writeCommand(char readWrite, char command, int id);
	void writeInt(int inInt);
	int readDxl(int id, char cmd);

	int fd;
};

#endif /* HARDWARE_H_ */
