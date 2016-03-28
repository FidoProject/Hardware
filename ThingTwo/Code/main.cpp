#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "Hardware.h"
#include "Fido/Fido.h"
#include "../../Connection.h"

void lSleep(double seconds) {
    usleep(seconds*1000000.0);
}

void goStraight() {
	double maxRotate = 100;
	double exploration = 0.2;

	Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

	rl::WireFitQLearn learner = rl::WireFitQLearn(1, 1, 1, 3, 4, {-1}, {1}, 11, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.1, 35000), 0.95, 0.4);
	learner.reset();
    
    std::cout << "Done with initialization\n";
	while(true) {
		rl::Action action = learner.chooseBoltzmanAction({1}, exploration);
        std::cout << "Action: " << action[0] << "\n";

		hardware.goHolonomic(0, 50, action[0]*maxRotate);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		hardware.goHolonomic(0, 0, 0);
	    
        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
		learner.applyReinforcementToLastAction(reward, {1});
	}

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({1}, 0.001);
        hardware.goHolonomic(0, 50, action[0]*maxRotate);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << "\n";
    }
}

void goStraightKiwi() {
    double maxMove = 100;
    double exploration = 0.2;

    Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

    rl::WireFitQLearn learner = rl::WireFitQLearn(1, 3, 1, 8, 4, {-1, -1, -1}, {1, 1, 1}, 3, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.1, 35000), 0.95, 0.4);
    learner.reset();
    
    std::cout << "Done with initialization\n";
    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({1}, exploration);
        std::cout << "Action: " << action[0] << "\n";

        hardware.setMotos(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        hardware.setMotors(0, 0, 0);
        
        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
        learner.applyReinforcementToLastAction(reward, {1});
    }

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({1}, 0.001);
        hardware.goHolonomic(0, 50, action[0]*maxRotate);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << "\n";
    }
}

int main() {
    goStraightKiwi();

    return 0;
}
