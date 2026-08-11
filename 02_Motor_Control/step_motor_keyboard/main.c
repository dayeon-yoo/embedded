#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>

#define BLUE_PIN 22
#define PINK_PIN 23
#define YELL_PIN 24
#define ORAG_PIN 25

#define B_LED_PIN 27
#define G_LED_PIN 28
#define R_LED_PIN 29

void setStep(int blue, int pink, int yell, int orag)
{
    digitalWrite(BLUE_PIN, blue);
    digitalWrite(PINK_PIN, pink);
    digitalWrite(YELL_PIN, yell);
    digitalWrite(ORAG_PIN, orag);
}

void motorStop(void)
{
    setStep(HIGH, HIGH, HIGH, HIGH);

    digitalWrite(B_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, LOW);
    digitalWrite(R_LED_PIN, LOW);
}

void motorCW(int steps)
{
    digitalWrite(R_LED_PIN, HIGH);
    digitalWrite(B_LED_PIN, LOW);

    for (int i = 0; i < steps; i++)
    {
        setStep(HIGH, HIGH, LOW, LOW);
        delay(2);

        setStep(LOW, HIGH, HIGH, LOW);
        delay(2);

        setStep(LOW, LOW, HIGH, HIGH);
        delay(2);

        setStep(HIGH, LOW, LOW, HIGH);
        delay(2);
    }

    motorStop();
}

void motorCCW(int steps)
{
    digitalWrite(B_LED_PIN, HIGH);
    digitalWrite(R_LED_PIN, LOW);

    for (int i = 0; i < steps; i++)
    {
        setStep(HIGH, LOW, LOW, HIGH);
        delay(2);

        setStep(LOW, LOW, HIGH, HIGH);
        delay(2);

        setStep(LOW, HIGH, HIGH, LOW);
        delay(2);

        setStep(HIGH, HIGH, LOW, LOW);
        delay(2);
    }

    motorStop();
}

int main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    pinMode(BLUE_PIN, OUTPUT);
    pinMode(PINK_PIN, OUTPUT);
    pinMode(YELL_PIN, OUTPUT);
    pinMode(ORAG_PIN, OUTPUT);

    pinMode(B_LED_PIN, OUTPUT);
    pinMode(G_LED_PIN, OUTPUT);
    pinMode(R_LED_PIN, OUTPUT);

    motorStop();

    printf("Enter rotation angle.\n");
    printf("Positive value : CW\n");
    printf("Negative value : CCW\n");
    printf("0 : STOP\n");
    printf("q : Quit\n");

    while (1)
    {
        char input[32];

        printf("\nAngle: ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        if (input[0] == 'q')
        {
            motorStop();
            break;
        }

        int angle = atoi(input);

        if (angle == 0)
        {
            motorStop();
            printf("Motor STOP\n");
            continue;
        }

        /*
         * The step count is calculated from the requested angle.
         * 512 steps are used as one full rotation reference.
         */
        int steps = abs(angle) * 512 / 360;

        if (steps <= 0)
            steps = 1;

        if (angle > 0)
        {
            printf("Motor CW: %d degrees\n", angle);
            motorCW(steps);
        }
        else
        {
            printf("Motor CCW: %d degrees\n", abs(angle));
            motorCCW(steps);
        }
    }

    motorStop();

    return 0;
}