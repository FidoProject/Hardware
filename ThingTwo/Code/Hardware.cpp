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

#define PIN_LINE_LED 12

#define PIN_BUZZER 11

const int Hardware::LINE_SENSORS[] = {6,7,5,3,4,2,0,1};

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

    // initialize line sensor array LED
    pinMode(PIN_LINE_LED,OUTPUT);

    // open serial line
    serialFileDesc = serialOpen("/dev/ttyAMA0",115200);

    // initialize buzzer
    softToneCreate(PIN_BUZZER);
}

void Hardware::setLED(int r, int g, int b) {
    softPwmWrite(PIN_LED_R,r);
    softPwmWrite(PIN_LED_G,g);
    softPwmWrite(PIN_LED_B,b);
}

int Hardware::readLine() {
    digitalWrite(PIN_LINE_LED,1);
    usleep(200);

    long long longest = 0;
    int longPin = 0;
    for (int i=0; i<8; i++) {
        long long iTime = (readLineSensor(LINE_SENSORS[i]) + readLineSensor(LINE_SENSORS[i]))/2;
        if (iTime > longest) {
            longest = iTime;
            longPin = i;
        }
    } if (longest > 1000) lastLine = longPin;
    
    return lastLine;
}

long long Hardware::readLineSensor(int i) {
    pinMode(i,OUTPUT);
    digitalWrite(i,1);
    usleep(10);

    pinMode(i,INPUT);
    pullUpDnControl(i,PUD_OFF);

    auto start = std::chrono::high_resolution_clock::now();
    while (digitalRead(i));
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

void Hardware::goHolonomic(int x, int y, int r) {
    double i = -x/2 - y*sqrt(3)/2 + r;
    double j = -x/2 + y*sqrt(3)/2 + r;
    double k = x + r;

    setMotors(i,j,k);
}

void Hardware::setMotors(int i, int j, int k) {
    digitalWrite(PIN_MOTOR_I_DIR,i>0);
    softPwmWrite(PIN_MOTOR_I_PWM,abs(i));

    digitalWrite(PIN_MOTOR_J_DIR,j>0);
    softPwmWrite(PIN_MOTOR_J_PWM,abs(j));

    digitalWrite(PIN_MOTOR_K_DIR,k<0);
    softPwmWrite(PIN_MOTOR_K_PWM,abs(k));
}

void Hardware::getZX(int& z, int& x) {
    serialFlush(serialFileDesc);

    while (serialGetchar(serialFileDesc) != 0xFA);
    x = serialGetchar(serialFileDesc) - 120;

    while (serialGetchar(serialFileDesc) != 0xFB);
    z = serialGetchar(serialFileDesc);
}

void Hardware::setBuzzer(int freq) {
    softToneWrite(PIN_BUZZER,freq);
}

Hardware::~Hardware() {
    setLED(0,0,0);
    setMotors(0,0,0);
    digitalWrite(PIN_LINE_LED,0);
    serialClose(serialFileDesc);
    setBuzzer(0);

    usleep(100000);
}
