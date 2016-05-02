#include "Hardware.h"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "../../../Fido/include/Fido.h"
#include "../../Connection.h"

#define I_MIN_SAFE_ANG -65
#define I_MAX_SAFE_ANG 65

#define J_MIN_SAFE_ANG 35
#define J_MAX_SAFE_ANG 180

#define K_MIN_SAFE_ANG 0
#define K_MAX_SAFE_ANG 90

double randOutput() {
	return ((double) rand() / (RAND_MAX))*2.0 - 1.0;
}

bool executeScaledAngles(Hardware *hardware, double i, double j, double k) {
	double iAng = Hardware::map(i, -1, 1, I_MIN_SAFE_ANG, I_MAX_SAFE_ANG);
	double jAng = Hardware::map(j, -1, 1, J_MIN_SAFE_ANG, J_MAX_SAFE_ANG);
	double kAng = Hardware::map(k, -1, 1, K_MIN_SAFE_ANG, K_MAX_SAFE_ANG);

	return hardware->setJoints(iAng, jAng, kAng);
}

void stressTest(Hardware *hardware, int trials) {
	for (int i=0; i<trials; i++) {
		if (executeScaledAngles(hardware,randOutput(),randOutput(),randOutput())) {
			std::cout << "Move executed.\n";
		} else {
			std::cout << "Move fail-safed.\n";
		} usleep(3000000);
	}
}

void testSonars(Hardware *hardware, int trials) {
	for (int i=0; i<trials; i++) {
		int l, r; hardware->getSonars(&l, &r);
		std::cout << "Sonars: (" << l << ", " << r << ")\n";
		std::cout.flush();
	}
}

void proceduralPingPong(Hardware *hand) {
	int THRESH = 50;
	int NUM_SONAR_READINGS = 1;

	hand->setJoints(0, 180, 45);

	while (true) {
		int l, r;
		for (int j=0; j<NUM_SONAR_READINGS; j++) {
			int tempL, tempR; hand->getSonars(&tempL, &tempR);
			l += tempL; r += tempR;
		} l /= NUM_SONAR_READINGS; r /= NUM_SONAR_READINGS;
		//std::cout << "Sonars: (" << l << "," << r << ")\n";

		int delay = 0;
		double i = 0;
		if (abs(r-l) > THRESH) {
			if (r > l) {
				std::cout << "RIGHT\n";
				i = -30;
			} else {
				std::cout << "LEFT\n";
				i = 30;
			} delay = 500000;
		} else std::cout << "NOTHING\n";

		//hand->setJoints(i, 180, 40, true);
		//usleep(delay);
	}
}

void proceduralDrawing(Hardware *hand) {
	hand->setJoints(-6, 95, 50);
    usleep(500000);
	hand->setJoints(6, 95, 50);
	usleep(500000);
	hand->setJoints(6, 100, 30);
	usleep(500000);
	hand->setJoints(-6, 100, 30);
	usleep(500000);
	hand->setJoints(-6, 95, 50);
    usleep(500000);
	hand->poise();
}

void drawSquare() {
	std::vector< std::vector<int> > points = {{-6, 95, 50}, {6, 95, 50}, {6, 100, 30}, {-6, 100, 30}};
	Connection connection;
	int receiverNum;
        do {
                    receiverNum = atoi(connection.getString().c_str());
                        } while(receiverNum != 5 && receiverNum != 6);

    Hardware hand;
	rl::FidoControlSystem learner(1, {-1}, {1}, 3);

	int currentIndex = 0;
	hand.poise();
    hand.setJoints(points[currentIndex][0], points[currentIndex][1], points[currentIndex][2]);
	while(true) {
		int offset = (int)(learner.chooseBoltzmanActionDynamic({1})[0]);
        std::cout << "ACTION: " << offset << "\n";
        int gg = currentIndex+offset;
        if(gg < 0) gg = 3
        if(gg > 3) gg = 0;
		hand.setJoints(points[gg][0], points[gg][1], points[gg][2]);
        std::cout << "DONEwith this\n"; std::cout.flush();
        for(int a = 0; a < 4; a++) {
            std::cout << "best action: " << int(learner.chooseBoltzmanAction({1}, 0)[0]) << "\n";
        }
        std::cout << "GIVE: " << (1 - fabs(1-offset)) << "\n";
        std::cout.flush();
        
        double reward = connection.getReward();

		if (fabs(reward) < 0.001) hand.neutral();
		else if (reward > 0) hand.good();
		else if (reward < 0) hand.bad();
		else if (fabs(reward - (-2)) < 0.001) break;
		
		learner.applyReinforcementToLastAction(reward, {1});
	    currentIndex ++;
    }
    int current = 0;
    while(true) {
        hand.setJoints(points[current+int(learner.chooseBoltzmanAction({1}, 0)[0])][0], points[currentIndex+int(learner.chooseBoltzmanAction({1}, 0)[0])][1], points[currentIndex+int(learner.chooseBoltzmanAction({1}, 0)[0])][2]);
        current += int(learner.chooseBoltzmanAction({1}, 0)[0]);
        current %= 4;
    }
}

int main() {
	srand(time(NULL));
    drawSquare();
}
