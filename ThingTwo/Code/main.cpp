#include <iostream>
#include <unistd.h>

#include "Hardware.h"

void lSleep(double seconds) {
    usleep(seconds*1000000.0);
}

int main() {
    Hardware thingTwo;

    thingTwo.setMotors(100,0,0);
    usleep(0.5);
    thingTwo.setMotors(0,100,0);
    usleep(0.5);
    thingTwo.setMotors(0,0,100);
    usleep(0.5);

    return 0;
}
