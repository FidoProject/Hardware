#ifndef HARDWARE_H_
#define HARDWARE_H_

#define I_MIN_ANG -65
#define I_MAX_ANG 65

#define J_MIN_ANG 35
#define J_MAX_ANG 180

#define K_MIN_ANG 0
#define K_MAX_ANG 270

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
	bool setJoints(double i, double j, double k);

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
	void forwardKinematicsXY(double theta1, double theta2, double *x, double *y);

	int fd;
};

#endif /* HARDWARE_H_ */
