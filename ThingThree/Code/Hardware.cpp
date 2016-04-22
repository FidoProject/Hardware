#include "Hardware.h"

Hardware::Hardware() {
    wiringPiSetup();
}

Hardware::~Hardware() {
    usleep(100000);
}
