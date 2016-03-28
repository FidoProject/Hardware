#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "../Hardware.h"
#include "Fido/Fido.h"
#include "../../../Connection.h"

// ---------------------- Helper Functions -----------------------
bool isLeftOfLine(Hardware &hardware) {
    int lineVal = hardware.readLine();
    std::cout << "Line val: " << lineVal << "\n";
    return lineVal < 4;
}

// ---------------------- Experiments ----------------------------

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
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";

        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        hardware.setMotors(0, 0, 0);

        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
        learner.applyReinforcementToLastAction(reward, {1});
    }

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({1}, 0.001);
        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";
    }
}

void lineFollow() {
    double maxDisplacementComponent = 100;
    double exploration = 0.2;

    Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

    rl::WireFitQLearn learner = rl::WireFitQLearn(1, 2, 1, 6, 4, {-1, -1}, {1, 1}, 3, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.05, 5000), 1, 0.5);
    learner.reset();

    std::cout << "Done with initialization\n";
    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, exploration);
        std::cout << "Action: " << action[0] << "\n";

        hardware.goHolonomic(action[0]*maxDisplacementComponent, action[1]*maxDisplacementComponent, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        hardware.goHolonomic(0, 0, 0);

        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
        learner.applyReinforcementToLastAction(reward, {!isLeftOfLine(hardware) ? -1 : 1});
    }

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, 0.001);
        hardware.goHolonomic(action[0]*maxDisplacementComponent, action[1]*maxDisplacementComponent, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << "\n";
    }
}

void lineFollowKiwi() {
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
        rl::Action action = learner.chooseBoltzmanAction({!!isLeftOfLine(hardware) ? -1 : 1}, exploration);
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";
        std::cout << "Is left of line: " << isLeftOfLine(hardware) << "\n";

        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        hardware.setMotors(0, 0, 0);

        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
        learner.applyReinforcementToLastAction(reward, {!isLeftOfLine(hardware) ? -1 : 1});
    }

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, 0.001);
        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";
    }
}

void ballFollow() {
    double maxDisplacementComponent = 100;
    double exploration = 0.2;

    Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

    rl::WireFitQLearn learner = rl::WireFitQLearn(2, 2, 1, 6, 4, {-1, -1}, {1, 1}, 3, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.05, 5000), 1, 0.5);
    learner.reset();
    
    std::cout << "Done with initialization\n";
    while(true) {
        int x, z;
        hardware.getZX(&z, &x);

        rl::Action action = learner.chooseBoltzmanAction({z / 240.0, x / 120.0}, exploration);
        std::cout << "Action: " << action[0] << " " << action[1] << "\n";

        hardware.goHolonomic(action[0]*maxDisplacementComponent, action[1]*maxDisplacementComponent, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        hardware.goHolonomic(0, 0, 0);
        
        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;

        hardware.getZX(&z, &x);
        learner.applyReinforcementToLastAction(reward, {z / 240.0, x / 120.0});
    }

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, 0.001);
        hardware.goHolonomic(action[0]*maxDisplacementComponent, action[1]*maxDisplacementComponent, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << "\n";
    }
}

int main() {
    //lineFollow();

    Hardware hardware;
    while (true) {
        std::cout << "Gyro: " << hardware.getGyro() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
