#include "irpigpio.h"
#include <stdio.h>
#include <wiringPi.h>

#define IR_PIN 18
#define LED 27

#define BUTTON_0 0x102a4f46
#define BUTTON_1 0x8534077e

int deviceSetup()
{
    if (gpioInitialise() < 0 || wiringPiSetup() < 0)
    {
        return -1;
    }

    /* IR pin as input */
    gpioSetMode(IR_PIN, PI_INPUT);

    /* 5ms max gap after last pulse */
    gpioSetWatchdog(IR_PIN, 5);

    /* monitor IR level changes */
    gpioSetAlertFunc(IR_PIN, alert);

    return 0;
}

int main()
{
    long value;

    if (deviceSetup() < 0)
        return 1;

    pinMode(LED, OUTPUT);

    while (1)
    {
        value = readIR();

        switch (value)
        {
            case BUTTON_0:
                printf("Button 0 pressed, LED On\n");
                digitalWrite(LED, HIGH);
                break;

            case BUTTON_1:
                printf("Button 1 pressed, LED Off\n");
                digitalWrite(LED, LOW);
                break;
        }
    }

    gpioTerminate();

    return 0;
}