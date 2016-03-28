#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    for (int i=0; i<5; i++) {
        std::cout << "Reading " << i << ": " << thingTwo.readLine() << "\n";
        std::cout.flush();

        sleep(1);
    }

    return 0;
}
