#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <iostream>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>

struct color {
    int r, g, b;
};

class Hardware {
public:
    Hardware();
    void setLEDs(std::vector<color> colors);
    void setServos(int i, int j, int k);
    virtual ~Hardware();
private:
};

#endif /* HARDWARE_H_ */
