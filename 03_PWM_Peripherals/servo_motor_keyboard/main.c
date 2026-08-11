#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO_PIN 26
#define LED_PIN 27

#define SERVO_MIN 6
#define SERVO_MAX 24
#define SERVO_CENTER 15

char getch(void)
{
    char c;
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr);

    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    newattr.c_cc[VMIN] = 1;
    newattr.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return c;
}

void updateLED(int angle)
{
    int brightness;

    /*
     * Angle 6 -> brightness 0
     * Angle 24 -> brightness 255
     */
    brightness = (angle - SERVO_MIN) * 255 /
                 (SERVO_MAX - SERVO_MIN);

    softPwmWrite(LED_PIN, brightness);
}

int main(void)
{
    int angle = SERVO_CENTER;

    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!");
        return -1;
    }

    pinMode(SERVO_PIN, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO_PIN, 0, 200);

    pinMode(LED_PIN, SOFT_PWM_OUTPUT);
    softPwmCreate(LED_PIN, 0, 255);

    // Initial position: center
    softPwmWrite(SERVO_PIN, angle);
    updateLED(angle);

    printf("Servo Motor Keyboard Control\n");
    printf("q : Counter-clockwise\n");
    printf("w : Center\n");
    printf("e : Clockwise\n");
    printf("q/e range : 6 ~ 24\n");

    while (1)
    {
        char key = getch();

        switch (key)
        {
            case 'q':
                if (angle < SERVO_MAX)
                    angle++;

                softPwmWrite(SERVO_PIN, angle);
                updateLED(angle);

                printf("Servo angle : %d\n", angle);
                break;

            case 'w':
                angle = SERVO_CENTER;

                softPwmWrite(SERVO_PIN, angle);
                updateLED(angle);

                printf("Servo angle : %d\n", angle);
                break;

            case 'e':
                if (angle > SERVO_MIN)
                    angle--;

                softPwmWrite(SERVO_PIN, angle);
                updateLED(angle);

                printf("Servo angle : %d\n", angle);
                break;

            default:
                break;
        }
    }

    softPwmStop(SERVO_PIN);
    softPwmStop(LED_PIN);

    return 0;
}