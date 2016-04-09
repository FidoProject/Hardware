#include <vector>

std::vector<int> helloWorld;
int i = 0;

void setup() {
	Serial.begin(9600);
	pinMode(13, OUTPUT);
}

void loop() {
	i++;
	helloWorld.push_back(i);

	digitalWrite(13, HIGH);   // set the LED on
	delay(1000);              // wait for a second
	digitalWrite(13, LOW);    // set the LED off
	delay(1000);              // wait for a second

	Serial.println(helloWorld.back());
}
