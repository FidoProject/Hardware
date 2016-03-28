#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    thingTwo.goHolonomic(100,0,0);
    sleep(1);
    thingTwo.goHolonomic(0,-100,0);
    sleep(1);
    thingTwo.setMotors(-100,0,0);
    sleep(2);
    thingTwo.setMotors(0,100,0);
    sleep(2);

    return 0;
}
