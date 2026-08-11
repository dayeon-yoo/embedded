#include <stdio.h>
#include <wiringPi.h>
#include "TM1637.h"

#define CLK_PIN 9
#define DIO_PIN 8

int main(void)
{
    // Numbers to display on the FND
    int numTable[] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    int displayData[4];
    unsigned int index = 0;
    unsigned int count = 0;

    delay(150);

    // Initialize wiringPi
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    // Configure TM1637 pins
    pinMode(CLK_PIN, INPUT);
    pinMode(DIO_PIN, INPUT);

    delay(200);

    // Initialize TM1637
    TM1637_init(CLK_PIN, DIO_PIN);

    // Set FND brightness
    TM1637_set(BRIGHTEST, 0x40, 0xC0);

    while (1)
    {
        unsigned char bitSelect = 0;

        count++;

        if (count == sizeof(numTable) / sizeof(numTable[0]))
        {
            count = 0;
        }

        // Prepare four digits for display
        for (bitSelect = 0; bitSelect < 4; bitSelect++)
        {
            displayData[bitSelect] = numTable[index];

            index++;

            if (index == sizeof(numTable) / sizeof(numTable[0]))
            {
                index = 0;
            }
        }

        // Display four digits
        TM1637_display(0, displayData[0]);
        TM1637_display(1, displayData[1]);
        TM1637_display(2, displayData[2]);
        TM1637_display(3, displayData[3]);

        delay(500);
    }

    return 0;
}