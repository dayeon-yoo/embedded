#include <stdio.h>
#include <wiringPi.h>

#define SWITCH_PIN 0
#define BUZ_A_PIN 30

int main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    pinMode(SWITCH_PIN, INPUT);
    pinMode(BUZ_A_PIN, OUTPUT);

    // Initial state: buzzer OFF
    digitalWrite(BUZ_A_PIN, LOW);

    int buzzerState = LOW;
    int previousSwitchState = HIGH;

    while (1)
    {
        int currentSwitchState = digitalRead(SWITCH_PIN);

        // Detect a button press
        if (previousSwitchState == HIGH &&
            currentSwitchState == LOW)
        {
            if (buzzerState == LOW)
            {
                buzzerState = HIGH;
                digitalWrite(BUZ_A_PIN, HIGH);
                printf("Buzzer ON\n");
            }
            else
            {
                buzzerState = LOW;
                digitalWrite(BUZ_A_PIN, LOW);
                printf("Buzzer OFF\n");
            }

            // Simple debounce
            delay(300);
        }

        previousSwitchState = currentSwitchState;
    }

    return 0;
}