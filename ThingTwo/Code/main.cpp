#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    thingTwo.setMotors(100,100,100);
    sleep(2);


    return 0;
}
