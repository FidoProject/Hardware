#include "Hardware.h"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#define I_MIN_SAFE_ANG -65
#define I_MAX_SAFE_ANG 65

#define J_MIN_SAFE_ANG 35
#define J_MAX_SAFE_ANG 120

#define K_MIN_SAFE_ANG 0
#define K_MAX_SAFE_ANG 180


bool executeScaledAngles(Hardware *hardware, int i, int j, int k) {
	double iAng = Hardware::map(i, -1, 1, I_MIN_SAFE_ANG, I_MAX_SAFE_ANG);
	double jAng = Hardware::map(j, -1, 1, J_MIN_SAFE_ANG, J_MAX_SAFE_ANG);
	double kAng = Hardware::map(k, -1, 1, K_MIN_SAFE_ANG, K_MAX_SAFE_ANG);

	return hardware->setJoints(iAng, jAng, kAng);
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

int main() {
	Hardware hand;
	hand.poise();

	stressTest(&hand, 10);
}
