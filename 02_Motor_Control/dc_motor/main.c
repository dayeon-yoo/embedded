#include <wiringPi.h>
#include <stdio.h>

// DC Motor 연결 핀
#define DCMotorA 6
#define DCMotorB 31

int main()
{
    // wiringPi 초기화
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!");
        return -1;
    }

    // DC Motor 핀 출력 설정
    pinMode(DCMotorA, OUTPUT);
    pinMode(DCMotorB, OUTPUT);

    while (1)
    {
        // 반시계 방향 (CCW)
        digitalWrite(DCMotorA, HIGH);
        digitalWrite(DCMotorB, LOW);
        printf("Motor run CCW\n");
        delay(1000);

        // 시계 방향 (CW)
        digitalWrite(DCMotorA, LOW);
        digitalWrite(DCMotorB, HIGH);
        printf("Motor run CW\n");
        delay(1000);

        // 정지
        digitalWrite(DCMotorA, LOW);
        digitalWrite(DCMotorB, LOW);
        printf("Motor run STOP\n");
        delay(1000);
    }

    return 0;
}