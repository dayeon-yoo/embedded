#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>

void hw_init(void);
void initBT(void);

int g_hBT;

int main(void)
{
    unsigned int nNextTime;

    initBT();
    hw_init();

    nNextTime = millis();

    while (1) {
        // 1초 간격으로 가상의 센서 데이터 전송
        if (millis() > nNextTime + 1000) {
            printf("Send Sensor Data\n");

            serialPrintf(g_hBT, "@TMP,32#");

            nNextTime = millis();
        }
    }

    return 0;
}

void hw_init(void)
{
    wiringPiSetup();
}

void initBT(void)
{
    g_hBT = serialOpen("/dev/rfcomm0", 115200);

    if (g_hBT < 0) {
        printf("Bluetooth Open Failed\n");
        return;
    }

    printf("Bluetooth init\n");
}