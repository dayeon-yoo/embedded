#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiSPI.h>

// ---------------------- SPI ADC ----------------------

#define CHAN_CONFIG_SINGLE    8
#define SPI_SPEED             1000000
#define SPI_CE_CHANNEL        1

#define SPI_ADC_LIGHT_CHANNEL 0
#define SPI_ADC_POTEN_CHANNEL 1

#define CS_MCP3208            11

// ---------------------- Ultrasonic ----------------------

#define ULTRASONIC_TRIG       4
#define ULTRASONIC_ECHO       5

// ---------------------- Bluetooth ----------------------

#define BT_DEVICE             "/dev/rfcomm0"
#define BT_BAUDRATE           115200

int g_hBT = -1;

// ---------------------- Function Prototypes ----------------------

void hw_init(void);
void InitBT(void);

int spiSetup(void);
int readAnalogData(int adcChannel);

float getUltrasonicDistance(void);
void sendSensorData(int ultra, int light, int poten);

// ============================================================
// main
// ============================================================

int main(void)
{
    unsigned int nSensorTime;

    InitBT();
    hw_init();

    nSensorTime = millis();

    while (1) {

        // 1초마다 센서 데이터 측정 및 전송
        if (millis() > nSensorTime + 1000) {

            int illum;
            int poten;
            int ultra_cm;

            illum = readAnalogData(SPI_ADC_LIGHT_CHANNEL);
            poten = readAnalogData(SPI_ADC_POTEN_CHANNEL);
            ultra_cm = (int)getUltrasonicDistance();

            printf(
                "Sensor Data - ULTRA=%d, LIGHT=%d, POTEN=%d\n",
                ultra_cm,
                illum,
                poten
            );

            sendSensorData(
                ultra_cm,
                illum,
                poten
            );

            nSensorTime = millis();
        }

        delay(10);
    }

    return 0;
}

// ============================================================
// Hardware initialization
// ============================================================

void hw_init(void)
{
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "wiringPiSetup Failed\n");
        exit(1);
    }

    // SPI 초기화
    if (spiSetup() != 0) {
        fprintf(stderr, "SPI setup failed\n");
        exit(1);
    }

    // MCP3208 CS
    pinMode(CS_MCP3208, OUTPUT);
    digitalWrite(CS_MCP3208, HIGH);

    // 초음파 센서
    pinMode(ULTRASONIC_TRIG, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);

    digitalWrite(ULTRASONIC_TRIG, LOW);
}

// ============================================================
// Bluetooth initialization
// ============================================================

void InitBT(void)
{
    g_hBT = serialOpen(BT_DEVICE, BT_BAUDRATE);

    if (g_hBT < 0) {
        fprintf(
            stderr,
            "Unable to open Bluetooth device: %s\n",
            strerror(errno)
        );

        exit(1);
    }

    printf("Bluetooth init OK (%s)\n", BT_DEVICE);
}

// ============================================================
// SPI setup
// ============================================================

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

// ============================================================
// Read analog data from MCP3208
// ============================================================

int readAnalogData(int adcChannel)
{
    int adcValue = 0;

    unsigned char buffer[3] = {0};

    // MCP3208 채널 범위 확인
    if (adcChannel < 0 || adcChannel > 7) {
        return -1;
    }

    // SPI 통신 버퍼 설정
    buffer[0] = 0x01;

    buffer[1] =
        (CHAN_CONFIG_SINGLE + adcChannel) << 4;

    // SPI 통신 활성화
    digitalWrite(CS_MCP3208, LOW);

    // ADC 데이터 읽기
    wiringPiSPIDataRW(
        SPI_CE_CHANNEL,
        buffer,
        3
    );

    // 10비트 ADC 데이터 추출
    adcValue =
        ((buffer[1] & 3) << 8) + buffer[2];

    // SPI 통신 비활성화
    digitalWrite(CS_MCP3208, HIGH);

    return adcValue;
}

// ============================================================
// Read ultrasonic distance
// ============================================================

float getUltrasonicDistance(void)
{
    unsigned int startTime;
    unsigned int endTime;
    unsigned int timeout;

    // Trigger pulse
    digitalWrite(ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG, LOW);

    // Echo HIGH 대기
    timeout = micros();

    while (digitalRead(ULTRASONIC_ECHO) == LOW) {

        if (micros() - timeout > 30000) {
            return -1.0;
        }
    }

    startTime = micros();

    // Echo LOW 대기
    timeout = micros();

    while (digitalRead(ULTRASONIC_ECHO) == HIGH) {

        if (micros() - timeout > 30000) {
            return -1.0;
        }
    }

    endTime = micros();

    // 초음파 왕복 시간을 거리(cm)로 변환
    return (endTime - startTime) / 58.0;
}

// ============================================================
// Send sensor data through Bluetooth
// ============================================================

void sendSensorData(
    int ultra,
    int light,
    int poten
)
{
    /*
     * Bluetooth transmission format
     *
     * @SENSOR,ULTRA,LIGHT,POTEN#
     */

    serialPrintf(
        g_hBT,
        "@SENSOR,%d,%d,%d#",
        ultra,
        light,
        poten
    );

    printf(
        "TX = @SENSOR,%d,%d,%d#\n",
        ultra,
        light,
        poten
    );
}