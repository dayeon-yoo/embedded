#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include "MFRC522.h"

#define SPI_SPEED 1000000
#define SPI_CE0_CHANNEL 0

int main(void)
{
    unsigned char data1[16] =
    {
        0x12, 0x34, 0x56, 0x78,
        0xED, 0xCB, 0xA9, 0x87,
        0x12, 0x34, 0x56, 0x78,
        0x01, 0xFE, 0x01, 0xFE
    };

    unsigned char DefaultKey[6] =
    {
        0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF
    };

    unsigned char g_ucTempbuf[20];

    int count = 0;
    unsigned char status;
    unsigned char i;

    /*
     * WiringPi initialization
     */
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    /*
     * Initialize SPI communication
     */
    if (wiringPiSPISetup(SPI_CE0_CHANNEL, SPI_SPEED) == -1)
    {
        fprintf(
            stderr,
            "wiringPiSPISetup Failed ! ERROR : %s\n",
            strerror(errno)
        );

        return 1;
    }

    delay(500);

    /*
     * Initialize MFRC522
     */
    PcdReset();
    PcdAntennaOff();

    delay(10);

    PcdAntennaOn();

    printf("... Touch RFID CARD\n\n");

    while (1)
    {
        /*
         * Detect RFID card
         */
        status = PcdRequest(PICC_REQALL, g_ucTempbuf);

        if (status != MI_OK)
            continue;

        /*
         * Print card type
         */
        printf("\ncard type:");

        switch (g_ucTempbuf[0])
        {
            case 0x02:
                printf("Mifare_One(S70)\n");
                break;

            case 0x04:
                printf("Mifare_One(S50)\n");
                break;

            case 0x08:
                printf("Mifare_Pro(X)\n");
                break;

            case 0x44:
                if (g_ucTempbuf[1] == 0x00)
                {
                    printf("Mifare_UltraLight\n");
                    break;
                }
                else if (g_ucTempbuf[1] == 0x03)
                {
                    printf("Mifare_DESFire\n");
                    break;
                }

            default:
                break;
        }

        /*
         * Get card serial number
         */
        status = PcdAnticoll(g_ucTempbuf);

        if (status != MI_OK)
            continue;

        printf("\ncard SN:");

        for (i = 0; i < 4; i++)
        {
            if (g_ucTempbuf[i] < 0x10)
                printf("0%X", g_ucTempbuf[i]);
            else
                printf("%X", g_ucTempbuf[i]);
        }

        printf("\n");

        /*
         * Select card
         */
        status = PcdSelect(g_ucTempbuf);

        if (status != MI_OK)
            continue;

        /*
         * Authenticate block 1
         */
        status = PcdAuthState(
            PICC_AUTHENT1A,
            1,
            DefaultKey,
            g_ucTempbuf
        );

        if (status != MI_OK)
            continue;

        /*
         * Write data to block 1
         */
        status = PcdWrite(1, data1);

        if (status != MI_OK)
            continue;

        /*
         * Back up value block 1 to block 2
         */
        status = PcdBakValue(1, 2);

        if (status != MI_OK)
            continue;

        /*
         * Read blocks 0 to 3
         */
        for (i = 0; i < 4; i++)
        {
            status = PcdRead(i, g_ucTempbuf);

            if (status != MI_OK)
                continue;

            printf("\nread block %d:", i);

            for (int j = 0; j < 16; j++)
            {
                if (g_ucTempbuf[j] < 0x10)
                    printf("0%X", g_ucTempbuf[j]);
                else
                    printf("%X", g_ucTempbuf[j]);
            }
        }

        printf("\n");

        /*
         * Halt RFID card
         */
        PcdHalt();

        delay(1000);

        /*
         * Run the example three times
         */
        if (count++ >= 3)
            break;
    }

    return 0;
}