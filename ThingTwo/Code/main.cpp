#include <iostream>
#include <unistd.h>

#include "Hardware.h"

int main() {
    Hardware thingTwo;

    int cMajor[] = {262,294,330,349,392,440,494};

    for (int i=0; i<7; i++) {
        thingTwo.setBuzzer(cMajor[i]);
        usleep(500000);
    } for (int i=5; i>=0; i--) {
        thingTwo.setBuzzer(cMajor[i]);
        usleep(500000);
    }

    return 0;
}
