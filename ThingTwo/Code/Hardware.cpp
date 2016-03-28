#include "Hardware.h"

#define PIN_LED_R 14
#define PIN_LED_G 13
#define PIN_LED_B 10

#define PIN_MOTOR_I_PWM 22
#define PIN_MOTOR_I_DIR 27
#define PIN_MOTOR_J_PWM 24
#define PIN_MOTOR_J_DIR 28
#define PIN_MOTOR_K_PWM 25
#define PIN_MOTOR_K_DIR 29

Hardware::Hardware() {
    wiringPiSetup();

    /// initialize LED software pwms
    softPwmCreate(PIN_LED_R,0,255);
    softPwmCreate(PIN_LED_G,0,255);
    softPwmCreate(PIN_LED_B,0,255);

    /// initialize motor software pwms
    softPwmCreate(PIN_MOTOR_I_PWM,0,100);
    softPwmCreate(PIN_MOTOR_J_PWM,0,100);
    softPwmCreate(PIN_MOTOR_K_PWM,0,100);

    /// initialize motor direction outputs
    pinMode(PIN_MOTOR_I_DIR,OUTPUT);
    pinMode(PIN_MOTOR_J_DIR,OUTPUT);
    pinMode(PIN_MOTOR_K_DIR,OUTPUT);
}

void Hardware::setLED(int r, int g, int b) {
    softPwmWrite(PIN_LED_R,r);
    softPwmWrite(PIN_LED_G,g);
    softPwmWrite(PIN_LED_B,b);
}

void Hardware::goHolonomic(int x, int y, int r) {
    double i = y*sqrt(3)/2 + x/2 + r;
    double j = y*sqrt(3)/2 - x/2 - r;
    double k = x - r;

    double coef = 100.0/(((i>j)?i:j)>k?((i>j)?i:j):k);

    setMotors(i*coef,j*coef,k*coef);
}

void Hardware::setMotors(int i, int j, int k) {
    digitalWrite(PIN_MOTOR_I_DIR,i>0);
    softPwmWrite(PIN_MOTOR_I_PWM,abs(i));

    digitalWrite(PIN_MOTOR_J_DIR,j>0);
    softPwmWrite(PIN_MOTOR_J_PWM,abs(j));

    digitalWrite(PIN_MOTOR_K_DIR,k>0);
    softPwmWrite(PIN_MOTOR_K_PWM,abs(k));
}

Hardware::~Hardware() {
    setLED(0,0,0);
    setMotors(0,0,0);

    usleep(100000);
}
