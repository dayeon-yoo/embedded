#include <wiringPi.h>
#include <stdio.h>

#define BLUE_PIN 22
#define PINK_PIN 23
#define YELL_PIN 24
#define ORAG_PIN 25

void setStep(int _blue, int _pink, int _yell, int _orag)
{
    digitalWrite(BLUE_PIN, _blue);
    digitalWrite(PINK_PIN, _pink);
    digitalWrite(YELL_PIN, _yell);
    digitalWrite(ORAG_PIN, _orag);
}

int main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!");
        return -1;
    }

    pinMode(BLUE_PIN, OUTPUT);
    pinMode(PINK_PIN, OUTPUT);
    pinMode(YELL_PIN, OUTPUT);
    pinMode(ORAG_PIN, OUTPUT);

    while (1)
    {
        printf("---------- CW\n");

        for (int i = 0; i <= 512; i++)
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

        printf("---------- CCW\n");

        for (int i = 0; i <= 512; i++)
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

        printf("---------- STOP\n");

        setStep(HIGH, HIGH, HIGH, HIGH);
        delay(500);
    }

    setStep(HIGH, HIGH, HIGH, HIGH);
    delay(500);

    return 0;
}