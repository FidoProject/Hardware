#include <iostream>
#include <unistd.h>

#include "Hardware.h"

void lSleep(double seconds) {
    usleep(seconds*1000000.0);
}

int main() {
    Hardware thingTwo;

    thingTwo.goHolonomic(0,100,20);
    lSleep(3);

    return 0;
}
