#include "Hardware.h"

#define PIN_LED_R 10
#define PIN_LED_G 13
#define PIN_LED_B 14

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
