#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    while (true) {
        std::cout << thingTwo.readLine() << "\n";
        std::cout.flush();
        usleep(500000);
    }

    return 0;
}
