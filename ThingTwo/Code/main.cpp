#include <iostream>
#include "Hardware.h"

int main() {
    Hardware thingTwo;

    for (int i=0; i<5; i++) {
        std::cout << "RED";
        thingTwo.setLED(255,0,0);
        sleep(2);

        std::cout << "GREEN";
        thingTwo.setLED(0,255,0);
        sleep(2);

        std::cout << "BLUE";
        thingTwo.setLED(0,0,255);
        sleep(2);
    }

    return 0;
}
