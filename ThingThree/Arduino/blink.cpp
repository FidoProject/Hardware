#include "Arduino.h"

#include <vector>

int main() {
	init();

	Serial.begin(9600);
	pinMode(13, OUTPUT);

	std::vector<int> helloWorld;
	int i = 0;

	while (true) {
		helloWorld.push_back(i++);
		Serial.println(helloWorld.back());

		digitalWrite(13, HIGH);
		delay(1000);
		digitalWrite(13, LOW);
		delay(1000);
	}
}
