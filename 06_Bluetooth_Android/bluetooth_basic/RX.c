#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

char rx_buf[BUFSIZ] = "";
int g_hBT;

void hw_init(void);
void initBT(void);

int main(void)
{
    unsigned int nNextTime;

    initBT();
    hw_init();

    nNextTime = millis();

    while (1) {
        // 0.25초 간격으로 수신 데이터 확인
        if (millis() > nNextTime + 250) {
            int i = 0;

            memset(rx_buf, 0, sizeof(rx_buf));

            while (serialDataAvail(g_hBT) > 0) {
                rx_buf[i] = serialGetchar(g_hBT);

                // '#'을 명령의 끝으로 사용
                if (rx_buf[i] == '#') {

                    // '@'로 시작하는 데이터만 출력
                    if (rx_buf[0] == '@') {
                        printf("RX Data = %s\n", rx_buf);
                    }

                    break;
                }

                i++;

                if (i >= BUFSIZ - 1) {
                    break;
                }
            }

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