#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    for (int i=0; i<8; i++) {
        std::cout << "Reading " << i << ": " << thingTwo.readLineSensor(i) << "\n";
        std::cout.flush();
    }



    return 0;
}
