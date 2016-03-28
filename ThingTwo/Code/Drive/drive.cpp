#include <iostream>
#include <unistd.h>
#include <termios.h>

#include "../Hardware.h"

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

int main() {
    Hardware thingTwo;

    while (true) {
        char input = getch();
        int x=0, y=0, r=0;
        if (input == 'a') x -= 100;
        else if (input == 'd') x += 100;
        else if (input == 'w') y += 100;
        else if (input == 's') y -= 100;
        else if (input == 'q') r += 100;
        else if (input == 'e') r -= 100;
        else if (input == ' ') break;
        else x = y = r = 0;

        thingTwo.goHolonomic(x,y,r);
    }
}
