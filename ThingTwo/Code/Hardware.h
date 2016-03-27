#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <wiringPi.h>
#include <softPwm.h>

#include <iostream>

class Hardware {
public:
    Hardware();
    void setLED(int r, int g, int b);
    virtual ~Hardware();
};

#endif /* HARDWARE_H_ */
