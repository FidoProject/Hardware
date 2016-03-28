#include <iostream>
#include <unistd.h>

#include "Hardware.h"

void lSleep(double seconds) {
    usleep(seconds*1000000.0);
}

int main() {
    Hardware thingTwo;

    thingTwo.setMotors(100,0,0);
    lSleep(0.5);
    thingTwo.setMotors(0,100,0);
    lSleep(0.5);
    thingTwo.setMotors(0,0,100);
    lSleep(0.5);

    return 0;
}
