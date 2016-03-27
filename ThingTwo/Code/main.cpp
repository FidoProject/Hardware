#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    thingTwo.setLED(121,30,29);
    sleep(2);


    return 0;
}
