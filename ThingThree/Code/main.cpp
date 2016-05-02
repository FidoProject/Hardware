#include "Hardware.h"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

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
	int NUM_SONAR_READINGS = 3;

	hand->setJoints(0, 150, 60);

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

int main() {
	srand(time(NULL));
	Hardware hand;
	hand.poise();

	usleep(500000);

	hand.clap(3);

	usleep(500000);

	proceduralPingPong(&hand);
	//proceduralDrawing(&hand);
}
