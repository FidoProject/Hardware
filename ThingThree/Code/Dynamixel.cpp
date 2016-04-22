#include "Dynamixel.h"

void Dynamixel::direction(d) {
	digitalWrite(RPI_DIRECTION_PIN,d);
	usleep(RPI_DIRECTION_SWITCH_DELAY);
}
