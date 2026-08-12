#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <pcf8574.h>
#include <lcd.h>
#include "irpigpio.h"

/* IR Remote */
#define IR_PIN 18

#define BUTTON_OPEN  0x102a4f46
#define BUTTON_CLOSE 0x8534077e

/* LCD */
#define BASE_ADDR 100
#define RS_PIN BASE_ADDR
#define RW_PIN (BASE_ADDR + 1)
#define EN_PIN (BASE_ADDR + 2)
#define BL_PIN (BASE_ADDR + 3)
#define D4_PIN (BASE_ADDR + 4)
#define D5_PIN (BASE_ADDR + 5)
#define D6_PIN (BASE_ADDR + 6)
#define D7_PIN (BASE_ADDR + 7)

/* Passive Buzzer */
#define BUZZER_PIN 21

/* Melody */
int openMelody[] = {262, 330, 392};   // 도 미 솔
int closeMelody[] = {392, 330, 262};  // 솔 미 도

int main(void)
{
    long value;
    int fd;
    int doorState = 0;   // 0: closed, 1: open

    /* wiringPi initialization */
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    /* IR initialization */
    if (deviceSetup() < 0)
    {
        printf("IR Setup Fail ~!!\n");
        return -1;
    }

    /* LCD initialization */
    pcf8574Setup(BASE_ADDR, 0x27);

    pinMode(RW_PIN, OUTPUT);
    digitalWrite(RW_PIN, LOW);

    pinMode(BL_PIN, OUTPUT);
    digitalWrite(BL_PIN, HIGH);

    fd = lcdInit(
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
        printf("LCD Setup Fail ~!!\n");
        return -1;
    }

    /* Passive buzzer initialization */
    softToneCreate(BUZZER_PIN);

    lcdClear(fd);

    while (1)
    {
        value = readIR();

        /*
         * Button 1
         * Open the door
         */
        if (value == BUTTON_OPEN)
        {
            /* Already open -> do nothing */
            if (doorState == 1)
                continue;

            doorState = 1;

            lcdClear(fd);
            lcdPosition(fd, 0, 0);
            lcdPuts(fd, "Open the door");

            /* Ascending melody: Do - Mi - Sol */
            softToneWrite(BUZZER_PIN, openMelody[0]);
            delay(200);

            softToneWrite(BUZZER_PIN, openMelody[1]);
            delay(200);

            softToneWrite(BUZZER_PIN, openMelody[2]);
            delay(200);

            softToneWrite(BUZZER_PIN, 0);

            delay(600);

            lcdClear(fd);
        }

        /*
         * Button 2
         * Close the door
         */
        else if (value == BUTTON_CLOSE)
        {
            /* Already closed -> do nothing */
            if (doorState == 0)
                continue;

            doorState = 0;

            lcdClear(fd);
            lcdPosition(fd, 0, 0);
            lcdPuts(fd, "Close the door");

            /* Descending melody: Sol - Mi - Do */
            softToneWrite(BUZZER_PIN, closeMelody[0]);
            delay(200);

            softToneWrite(BUZZER_PIN, closeMelody[1]);
            delay(200);

            softToneWrite(BUZZER_PIN, closeMelody[2]);
            delay(200);

            softToneWrite(BUZZER_PIN, 0);

            delay(600);

            lcdClear(fd);
        }
    }

    softToneStop(BUZZER_PIN);
    lcdClear(fd);

    return 0;
}