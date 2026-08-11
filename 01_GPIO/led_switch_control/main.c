#include <stdio.h>
#include <wiringPi.h>

#define SWITCH_PIN 0

#define B_LED_PIN 27
#define G_LED_PIN 28
#define R_LED_PIN 29

void turnOffLEDs(void)
{
    digitalWrite(B_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, LOW);
    digitalWrite(R_LED_PIN, LOW);
}

void setLEDState(int state)
{
    turnOffLEDs();

    switch (state)
    {
        case 0:
            // All LEDs OFF
            printf("LED OFF\n");
            break;

        case 1:
            // Green
            digitalWrite(G_LED_PIN, HIGH);
            printf("GREEN LED ON\n");
            break;

        case 2:
            // Red
            digitalWrite(R_LED_PIN, HIGH);
            printf("RED LED ON\n");
            break;

        case 3:
            // Blue
            digitalWrite(B_LED_PIN, HIGH);
            printf("BLUE LED ON\n");
            break;
    }
}

int main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    pinMode(SWITCH_PIN, INPUT);

    pinMode(B_LED_PIN, OUTPUT);
    pinMode(G_LED_PIN, OUTPUT);
    pinMode(R_LED_PIN, OUTPUT);

    turnOffLEDs();

    int state = 0;
    int previousSwitchState = HIGH;

    while (1)
    {
        int currentSwitchState = digitalRead(SWITCH_PIN);

        // Detect a button press
        if (previousSwitchState == HIGH &&
            currentSwitchState == LOW)
        {
            state++;

            if (state > 3)
                state = 0;

            setLEDState(state);

            // Simple debounce
            delay(300);
        }

        previousSwitchState = currentSwitchState;
    }

    return 0;
}