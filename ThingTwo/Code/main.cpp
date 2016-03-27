#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    for (int i=0; i<5; i++) {
        std::cout << "RED"; std::cout.flush();
        thingTwo.setLED(255,0,0);
        sleep(2);

        std::cout << "GREEN"; std::cout.flush();
        thingTwo.setLED(0,255,0);
        sleep(2);

        std::cout << "BLUE"; std::cout.flush();
        thingTwo.setLED(0,0,255);
        sleep(2);
    }

    return 0;
}
