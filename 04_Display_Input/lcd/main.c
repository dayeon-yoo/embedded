#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <stdio.h>

#define BASE_ADDR 100

#define RS_PIN    BASE_ADDR
#define RW_PIN    (BASE_ADDR + 1)
#define EN_PIN    (BASE_ADDR + 2)
#define BL_PIN    (BASE_ADDR + 3)
#define D4_PIN    (BASE_ADDR + 4)
#define D5_PIN    (BASE_ADDR + 5)
#define D6_PIN    (BASE_ADDR + 6)
#define D7_PIN    (BASE_ADDR + 7)

int main(void)
{
    // Initialize wiringPi
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    // Initialize PCF8574 for I2C-based LCD
    pcf8574Setup(RS_PIN, 0x27);

    // Configure LCD R/W pin
    pinMode(RW_PIN, OUTPUT);
    digitalWrite(RW_PIN, LOW);

    // Configure LCD backlight
    pinMode(BL_PIN, OUTPUT);
    digitalWrite(BL_PIN, HIGH);

    // Initialize 16x2 LCD
    int fd = lcdInit(
        2, 16, 4,
        RS_PIN,
        EN_PIN,
        D4_PIN,
        D5_PIN,
        D6_PIN,
        D7_PIN,
        0, 0, 0, 0
    );

    if (fd < 0)
    {
        printf("LCD initialization failed.\n");
        return -1;
    }

    while (1)
    {
        // Clear LCD screen
        lcdClear(fd);
        delay(500);

        // Display message on the first line
        lcdPosition(fd, 2, 0);
        lcdPuts(fd, "Hello, World!");

        // Display message on the second line
        lcdPosition(fd, 2, 1);
        lcdPuts(fd, "Good Luck *");

        delay(2000);
    }

    lcdClear(fd);

    return 0;
}