#include "Hardware.h"

#define PIN_LED_R 14
#define PIN_LED_G 13
#define PIN_LED_B 10

Hardware::Hardware() {
    wiringPiSetup();

    softPwmCreate(PIN_LED_R,0,255);
    softPwmCreate(PIN_LED_G,0,255);
    softPwmCreate(PIN_LED_B,0,255);
}

void Hardware::setLED(int r, int g, int b) {
    softPwmWrite(PIN_LED_R,r);
    softPwmWrite(PIN_LED_G,g);
    softPwmWrite(PIN_LED_B,b);
}

Hardware::~Hardware() {
    setLED(0,0,0);

    usleep(100000);
}
