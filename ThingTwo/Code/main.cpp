#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "Hardware.h"
#include "Fido.h"
#include "../../Connection.h"

void lSleep(double seconds) {
    usleep(seconds*1000000.0);
}

void goStraight() {
	double maxRotate = 100;
	double exploration = 0.2;

	Connection connection;
	Hardware hardware;

	Simlink simulator;
	rl::WireFitQLearn learner = rl::WireFitQLearn(1, 1, 1, 4, 3, {-1}, {1}, 11, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.1, 35000), 0.95, 0.4);
	learner.reset();

	while(true) {
		rl::Action action = learner.chooseBoltzmanAction({1}, exploration);

		hardware.goHolonomic(0, 50, action[0]*maxRotate);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		hardware.goHolonomic(0, 0, 0);
		
		learner.applyReinforcementToLastAction(connection.getReward(), {1});
	}
}

int main() {
    Hardware thingTwo;

    thingTwo.goHolonomic(0,100,20);
    lSleep(3);

    return 0;
}
