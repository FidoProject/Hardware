#include "Dynamixel.h"

void Dynamixel::direction(d) {
	digitalWrite(RPI_DIRECTION_PIN,d);
	usleep(RPI_DIRECTION_SWITCH_DELAY);
}

int Dynamixel::readData(int id) {
	direction(RPI_DIRECTION_RX);
	std::vector<char> reply = getSerial(5);

	if (reply[0] != 0xFF)  {
		std::cout << "Servo " << id << "timeout error" << "\n";
		return -1;
	}

	int length = reply[3] - 2;
	int error = reply[4];

	if (error != 0) {
		std::cout << "Error from servo: " << dictErrors.find(error) << "\n";
		return -error;
	} else if (length == 0) return error;
	else {
		int returnValue;
		if (length > 1) {
			std::vector<char> newReply = getSerial(2);
			return (reply[1]<<8) + (reply[0]<<0);
		} else return serialGetchar(port);
	}
}

std::vector<char> getSerial(int bytes) {
	std::vector<char> reply;
	while (reply.size() < bytes) reply.push_back(serialGetchar(port));
	return reply;
}
