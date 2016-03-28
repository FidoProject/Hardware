#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <wiringSerial.h>
#include <softTone.h>

class Hardware {
public:
    Hardware();
    void setLED(int r, int g, int b);
    void setMotors(int i, int j, int k);
    void goHolonomic(int x, int y, int r);
    int readLine();
    void getZX(int& z, int& x);
    void setBuzzer(int freq);
    virtual ~Hardware();
private:
    static const int LINE_SENSORS[];
    long long readLineSensor(int i);
    int serialFileDesc;
};

#endif /* HARDWARE_H_ */
