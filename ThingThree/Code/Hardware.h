#ifndef HARDWARE_H_
#define HARDWARE_H_


class Hardware {
public:
    Hardware();

	void good();
	void bad();
	void neutral();

	void poise();
	void clap(int claps);
	void gripper(bool open);

	bool setEffectorPosition(double theta, double x, double y);
	bool setJointsUnsafe(double i, double j, double k, bool override);
	bool setJoints(double i, double j, double k, bool override);

	void setSpeed(int id, int speed);
	void setTorque(int id, int torque);

	void getSonars(int *sonarOne, int *sonarTwo);
	int getServoError(int id);

    virtual ~Hardware();

	static double map(double x, double in_min, double in_max, double out_min, double out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
private:
	void writeCommand(char readWrite, char command, int id);
	void writeInt(int inInt);
	int readDxl(int id, char cmd);
	void moveJoint(int id, int position);

	bool safetyCheck(double x, double y, double z);
	void scaleServos(double iAng, double jAng, double kAng, int *iVal, int *jVal, int *kVal);
	void inverseKinematicsXY(double x, double y, double *theta1, double *theta2);
	void forwardKinematicsXY(double theta0, double theta1, double theta2, double *x, double *y, double *z);

	int fd;
};

#endif /* HARDWARE_H_ */
