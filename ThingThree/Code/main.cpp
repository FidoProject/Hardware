#include "Hardware.h"

#include <unistd.h>
#include <iostream>

int main() {
	Hardware hand;

	hand.poise();

	usleep(1000000);

	std::cout << hand.setEffectorPosition(0, 0, 0);
}
