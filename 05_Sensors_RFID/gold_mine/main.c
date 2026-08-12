#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softTone.h>

#define CHAN_CONFIG_SINGLE 8

#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1

#define SPI_ADC_POTEN_CHANNEL 1
#define SPI_ADC_CDS_CHANNEL 0

#define CS_MCP3208 11

#define BUZ_P_PIN 21
#define R_LED_PIN 29

/*
 * CDS의 어두운 상태를 판단하기 위한 기준값.
 *
 * 과제에서는 "사람의 손으로 가렸을 때 도달하는 값"이라고만
 * 제시되어 있으므로 정확한 수치는 회로 환경에 따라 조정한다.
 */
#define CDS_DARK_THRESHOLD 300

/*
 * 가변저항 값이 키 값의 ±100 이내이면
 * 빨간색 LED를 켠다.
 */
#define LED_RANGE 100

/*
 * 가변저항 값이 키 값의 ±10 이내이면
 * 금고가 열린 것으로 판단한다.
 */
#define OPEN_RANGE 10


/* ----------------------------------------
 * SPI 초기화
 * ---------------------------------------- */

int spiSetup(void)
{
    if (wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED) == -1) {
        fprintf(
            stderr,
            "wiringPiSPISetup Failed! ERROR: %s\n",
            strerror(errno)
        );

        return -1;
    }

    return 0;
}


/* ----------------------------------------
 * MCP3208 ADC 데이터 읽기
 * ---------------------------------------- */

int readAnalogData(int adcChannel)
{
    int adcValue = 0;

    unsigned char buffer[3] = {0};

    /*
     * MCP3208은 0~7번까지 8개의 채널 사용
     */
    if (adcChannel < 0 || adcChannel > 7) {
        return -1;
    }

    /*
     * SPI 통신 버퍼 설정
     */
    buffer[0] = 0x01;
    buffer[1] = (CHAN_CONFIG_SINGLE + adcChannel) << 4;

    /*
     * SPI 통신 시작
     */
    digitalWrite(CS_MCP3208, LOW);

    wiringPiSPIDataRW(
        SPI_CE_CHANNEL,
        buffer,
        3
    );

    /*
     * 10-bit ADC 데이터 추출
     */
    adcValue = ((buffer[1] & 0x03) << 8) + buffer[2];

    /*
     * SPI 통신 종료
     */
    digitalWrite(CS_MCP3208, HIGH);

    return adcValue;
}


/* ----------------------------------------
 * 상승 음계 출력
 * ---------------------------------------- */

void playAscendingScale(void)
{
    unsigned int melody[8] = {
        262, 294, 330, 349,
        392, 440, 494, 523
    };

    int i;

    for (i = 0; i < 8; i++) {
        softToneWrite(BUZ_P_PIN, melody[i]);
        delay(200);
    }

    softToneWrite(BUZ_P_PIN, 0);
}


/* ----------------------------------------
 * 카메라 촬영
 * ---------------------------------------- */

void takePicture(void)
{
    printf("Take a Picture!\n");

    /*
     * Raspberry Pi Camera 촬영
     */
    system("libcamera-jpeg -o gold_mine.jpg -t 1000");

    printf("Picture saved.\n");
}


/* ----------------------------------------
 * Main
 * ---------------------------------------- */

int main(void)
{
    int keyValue;
    int potValue;
    int cdsValue;

    /*
     * wiringPi 초기화
     */
    if (wiringPiSetup() == -1) {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    /*
     * SPI 초기화
     */
    if (spiSetup() == -1) {
        return -1;
    }

    /*
     * MCP3208 Chip Select
     */
    pinMode(CS_MCP3208, OUTPUT);
    digitalWrite(CS_MCP3208, HIGH);

    /*
     * RGB LED - Red
     */
    pinMode(R_LED_PIN, OUTPUT);
    digitalWrite(R_LED_PIN, LOW);

    /*
     * Passive Buzzer
     */
    pinMode(BUZ_P_PIN, SOFT_TONE_OUTPUT);

    if (softToneCreate(BUZ_P_PIN) < 0) {
        printf("Buzzer Setup Fail ~!!\n");
        return -1;
    }

    softToneWrite(BUZ_P_PIN, 0);

    /*
     * 난수 초기화
     */
    srand((unsigned int)time(NULL));

    /*
     * 초기 상태:
     * 아무 동작도 하지 않고 Enter 입력을 기다린다.
     */
    printf("Press Enter to start the safe.\n");

    getchar();

    /*
     * 10~1000 사이의 정수형 난수를
     * 금고의 키 값으로 저장
     */
    keyValue = (rand() % 991) + 10;

    printf("Key Value = %d\n", keyValue);
    printf("Turn the potentiometer to find the key value.\n");

    /*
     * 금고 동작 시작
     */
    while (1) {

        /*
         * 가변저항 값 읽기
         */
        potValue = readAnalogData(SPI_ADC_POTEN_CHANNEL);

        if (potValue < 0) {
            printf("Potentiometer Read Error!\n");
            break;
        }

        /*
         * 가변저항 값 주기적 출력
         */
        printf("Potentiometer Value = %d\n", potValue);

        /*
         * 키 값과의 차이
         */
        int difference = abs(potValue - keyValue);

        /*
         * --------------------------------
         * ±100 범위
         * --------------------------------
         *
         * 키 값 근처에 들어오면
         * 빨간색 LED ON
         */
        if (difference <= LED_RANGE) {
            digitalWrite(R_LED_PIN, HIGH);
        }
        else {
            digitalWrite(R_LED_PIN, LOW);
        }

        /*
         * --------------------------------
         * ±10 범위
         * --------------------------------
         *
         * 금고가 열린다.
         */
        if (difference <= OPEN_RANGE) {

            printf("Safe Open!\n");

            /*
             * 빨간색 LED 유지
             */
            digitalWrite(R_LED_PIN, HIGH);

            /*
             * 상승 음계 출력
             */
            playAscendingScale();

            /*
             * --------------------------------
             * 금고가 열린 후 조도 확인
             * --------------------------------
             */
            cdsValue = readAnalogData(SPI_ADC_CDS_CHANNEL);

            if (cdsValue < 0) {
                printf("CDS Read Error!\n");
                break;
            }

            printf("CDS Value = %d\n", cdsValue);

            /*
             * 조도가 낮으면
             * 사람의 손으로 센서를 가린 것으로 판단
             */
            if (cdsValue < CDS_DARK_THRESHOLD) {

                printf("Dark environment detected.\n");

                /*
                 * 카메라 촬영
                 */
                takePicture();
            }
            else {

                printf("Normal light detected.\n");
            }

            /*
             * 모든 작업이 완료되면 종료
             */
            break;
        }

        /*
         * 500ms마다 센서 값 갱신
         */
        delay(500);
    }

    /*
     * 종료 시 장치 OFF
     */
    digitalWrite(R_LED_PIN, LOW);

    softToneWrite(BUZ_P_PIN, 0);
    softToneStop(BUZ_P_PIN);

    return 0;
}