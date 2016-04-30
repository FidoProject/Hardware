#include "Hardware.h"

#include <unistd.h>

int main() {
	Hardware hand;

	hand.poise();

	usleep(1000000);

	hand.setEffectorPosition(0, 10, 10);
}
