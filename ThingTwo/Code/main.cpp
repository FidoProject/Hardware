#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    while (true) {
        int z, x;
        thingTwo.getZX(z,x);

        std::cout << "Z: " << z << ", X: " << x << "\n";
        std::cout.flush();
        usleep(500000);
    }

    return 0;
}
