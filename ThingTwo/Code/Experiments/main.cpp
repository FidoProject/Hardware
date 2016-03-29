#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "../Hardware.h"
#include "Fido/Fido.h"
#include "../../../Connection.h"

int main() {
    Hardware hardware;
    while (true) {
        std::cout << "Gyro: " << hardware.getGyro() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } return 0;
}
