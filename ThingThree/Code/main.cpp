#include "Hardware.h"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

int main() {
	Hardware hand;
	hand.poise();

	stressTest(&hand, 10);
}

void stressTest(Hardware *hardware, int trials) {
	for (int i=0; i<trials; i++) {
		if (executeScaledAngles(hardware,rand()*2.0-1,rand()*2.0-1,rand()*2.0-1)) {
			std::cout << "Move executed.\n";
		} else {
			std::cout << "Move fail-safed.\n";
		} usleep(3000000);
	}
}

bool executeScaledAngles(Hardware *hardware, int i, int j, int k) {
	double iAng = Hardware::map(i, -1, 1, I_MIN_ANG, I_MAX_ANG);
	double jAng = Hardware::map(j, -1, 1, J_MIN_ANG, J_MAX_ANG);
	double kAng = Hardware::map(k, -1, 1, K_MIN_ANG, K_MAX_ANG);

	return hardware->setJoints(iAng, jAng, kAng);
}
