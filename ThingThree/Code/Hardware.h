#ifndef HARDWARE_H_
#define HARDWARE_H_

class Hardware {
public:
    Hardware();

	void poise();
	void clap(int claps);
	void gripper(bool open);

	void setEffectorPosition(double theta, double x, double y);
	void setJoints(double i, double j, double k);
	void setSpeed(int id, int speed);
	void setTorque(int id, int torque);
	int getError(int id);

    virtual ~Hardware();
private:
	void writeCommand(char readWrite, char command, int id);
	void writeInt(int inInt);
	int readDxl(int id, char cmd);
	void moveJoint(int id, int position);

	double map(double x, double in_min, double in_max, double out_min, double out_max);
	void scaleServos(double iAng, double jAng, double kAng, int *iVal, int *jVal, int *kVal);
	void inverseKinematicsXY(double x, double y, double *theta1, double *theta2);

	int fd;
};

#endif /* HARDWARE_H_ */
