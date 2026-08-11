#include <wiringPi.h>
#include <stdlib.h>

#define BUZ_A_PIN 30

int main(void)
{
    if (wiringPiSetup() == -1)
        exit(1);

    pinMode(BUZ_A_PIN, OUTPUT);

    while (1)
    {
        digitalWrite(BUZ_A_PIN, HIGH);
        delay(1000);

        digitalWrite(BUZ_A_PIN, LOW);
        delay(1000);
    }

    return 0;
}