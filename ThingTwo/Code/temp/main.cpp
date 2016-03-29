#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <string> 
#include <fstream>
#include <ctime>
#include <algorithm>

#include "../Hardware.h"
#include "Fido/Fido.h"
#include "../../../Connection.h"

// ---------------------- Helper Functions -----------------------
bool isLeftOfLine(Hardware &hardware) {
    int lineVal = hardware.readLine();
    std::cout << "Line val: " << lineVal << "\n";
    return lineVal < 4;
}

void printStats(std::vector<double> iterations) {
    double sum = 0;
    for(auto a = iterations.begin(); a != iterations.end(); a++) sum += *a;
    std::cout << "Average iter: " << (sum / double(iterations.size())) << "\n";
    std::sort(iterations.begin(), iterations.end(), [=](double a, double b) {
        return a < b;
    });
    std::cout << "Median iter: " << iterations[iterations.size() / 2] << "\n";
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
    double exploration = 0.66666;

    Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

    rl::WireFitQLearn learner = rl::WireFitQLearn(1, 3, 1, 8, 4, {-1, -1, -1}, {1, 1, 1}, 3, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.05, 5000), 0.95, 0.4);
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
        
        exploration *= .75;
        if(exploration < 0.2) exploration = 0.2;
    }
    
    std::ofstream ostream;
     ostream.open("straightkiwi_"+std::to_string(rand())+".txt");
    learner.store(&ostream);

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

    rl::WireFitQLearn learner = rl::WireFitQLearn(1, 2, 1, 6, 4, {-1, -1}, {1, 1}, 3, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.05, 5000), 1, 0);
    learner.reset();

    std::cout << "Done with initialization\n";
    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, exploration);
        std::cout << "Action: " << action[0] << " " << action[1] << "\n";

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
        std::cout << "Action: " << action[0] << " " << action[1] << "\n";
    }
}

void lineFollowKiwi() {
    std::vector<double> choosingTimes, updateTimes;
    double maxMove = 100;
    double exploration = 0.2;

    Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

    rl::WireFitQLearn learner = rl::WireFitQLearn(1, 3, 1, 8, 4, {-1, -1, -1}, {1, 1, 1}, 6, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.05, 5000), 1, 0);

    std::cout << "Done with initialization\n";
    while(true) {
        clock_t begin = clock();
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, exploration);
        choosingTimes.push_back(double(clock() - begin) / CLOCKS_PER_SEC);
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";
        std::cout << "Is left of line: " << isLeftOfLine(hardware) << "\n";

        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        hardware.setMotors(0, 0, 0);
        
        if(!isLeftOfLine(hardware)) {
            std::cout << "Give reward: " << 2-(fabs(action[0] - 0.933)+fabs(action[1]-0.669)+        fabs(action[2]+0.5))/3.0;std::cout.flush();
        } else {
            std::cout << "Give reward: " << 2-(fabs(action[0] - 0.0669)+fabs(action[1]-0.933)+fabs(action[2]-0.5))/3.0;std::cout.flush();
        }
    
        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
        begin = clock();
        learner.applyReinforcementToLastAction(reward, {!isLeftOfLine(hardware) ? -1 : 1});
        updateTimes.push_back(double(clock() - begin) / CLOCKS_PER_SEC);
    }
    
    printStats(choosingTimes);
    printStats(updateTimes);

    std::ofstream ostream;
    ostream.open(std::to_string(rand())+".txt");
    learner.store(&ostream);

    while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, 0.001);
        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";
    }
}

void ballFollow() {
    double maxDisplacementComponent = 100;
    double exploration = 0.3;

    Connection connection;
    int receiverNum;
    do {
        receiverNum = atoi(connection.getString().c_str());
    } while(receiverNum != 5 && receiverNum != 6);

    Hardware hardware;

    rl::WireFitQLearn learner = rl::WireFitQLearn(1, 1, 1, 8, 4, {-1}, {1}, 6, new rl::LSInterpolator(), net::Backpropagation(0.01, 0.9, 0.05, 5000), 1, 0);
    learner.reset();
    
    std::cout << "Done with initialization\n";
    while(true) {
        int x, z;
        hardware.getZX(z, x);
        
        std::cout << " x: " << x << "\n";

        rl::Action action = learner.chooseBoltzmanAction({x < 0}, exploration);
        std::cout << "Action: " << action[0] << "\n";

        hardware.goHolonomic(0, 100, action[0]*maxDisplacementComponent);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        hardware.setMotors(0, 0, 0);
        
        std::cout << "Get reward\n"; std::cout.flush();
        double reward = connection.getReward();
        if(fabs(reward - (-2)) < 0.001) break;
 
        hardware.getZX(z, x);
        
        std::cout << "Training...\n";std::cout.flush();
        learner.applyReinforcementToLastAction(reward, {x < 0});
        std::cout << "Done training...\n";std::cout.flush();
    }
    
    while(true) {
        int z, x; 
        hardware.getZX(z, x);
        rl::Action action = learner.chooseBoltzmanAction({x < 0}, 0.001);
        hardware.goHolonomic(0, 100, action[0]*maxDisplacementComponent);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << "\n";
    }
}

void runModel(std::string filename) {
    Hardware hardware;
    double maxMove = 100;
    std::ifstream istream;
    istream.open(filename);
    rl::WireFitQLearn learner = rl::WireFitQLearn(&istream);

     while(true) {
        rl::Action action = learner.chooseBoltzmanAction({!isLeftOfLine(hardware) ? -1 : 1}, 0.001);
        hardware.setMotors(action[0]*maxMove, action[1]*maxMove, action[2]*maxMove);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Action: " << action[0] << " " << action[1] << " " << action[2] << "\n";
    }
}

int main() {
    srand(time(NULL));
    //runModel("2016764524.txt");
    ballFollow();
    
    /*Hardware h;
    int z, x;
    while(true) {
        h.getZX(z, x);
        std::cout << z << ", " << x << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }*/
    return 0;
}
