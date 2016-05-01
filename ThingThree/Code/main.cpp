#include "Hardware.h"

#include <unistd.h>
#include <iostream>

int main() {
	Hardware hand;

	hand.poise();
	
	usleep(1000000);
	hand.bad();
	usleep(1000000);
	hand.good();
	usleep(1000000);
	hand.neutral();
}
