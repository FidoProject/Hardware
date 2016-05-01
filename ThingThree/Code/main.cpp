#include "Hardware.h"

#include <unistd.h>
#include <iostream>

int main() {
	Hardware hand;
	hand.poise();

	int a, b;
	hand.getSonars(&a, &b);
	std::cout << "Sonar Left: " << a << "\n";
	std::cout << "Sonar Right: " << b << "\n";
}
