#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <iostream>
#include <unistd.h>

#include <wiringPi.h>
#include <softPwm.h>

class Hardware {
public:
    Hardware();
    void setLED(int r, int g, int b);
    virtual ~Hardware();
};

#endif /* HARDWARE_H_ */
