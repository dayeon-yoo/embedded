#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>

#define DCMotorA 6
#define DCMotorB 31

char getch(void)
{
    char c;
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr);

    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return c;
}

void motorStop(void)
{
    digitalWrite(DCMotorA, LOW);
    digitalWrite(DCMotorB, LOW);
    printf("Motor STOP\n");
}

void motorCW(void)
{
    digitalWrite(DCMotorA, LOW);
    digitalWrite(DCMotorB, HIGH);
    printf("Motor CW\n");
}

void motorCCW(void)
{
    digitalWrite(DCMotorA, HIGH);
    digitalWrite(DCMotorB, LOW);
    printf("Motor CCW\n");
}

int main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    pinMode(DCMotorA, OUTPUT);
    pinMode(DCMotorB, OUTPUT);

    motorStop();

    printf("DC Motor Keyboard Control\n");
    printf("+ : CW\n");
    printf("- : CCW\n");
    printf("0 : STOP\n");
    printf("q : Quit\n");

    while (1)
    {
        char key = getch();

        switch (key)
        {
            case '+':
                motorCW();
                break;

            case '-':
                motorCCW();
                break;

            case '0':
                motorStop();
                break;

            case 'q':
                motorStop();
                printf("Program terminated.\n");
                return 0;

            default:
                break;
        }
    }

    return 0;
}