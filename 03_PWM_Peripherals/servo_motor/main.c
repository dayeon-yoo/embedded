#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define SERVO_PIN 26

int main(void)
{
    // wiringPi 초기화
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!");
        return -1;
    }

    // Soft PWM 출력 설정
    pinMode(SERVO_PIN, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO_PIN, 0, 200);

    while (1)
    {
        // 15도 방향 이동
        softPwmWrite(SERVO_PIN, 15);
        delay(500);

        // 24도 방향 이동
        softPwmWrite(SERVO_PIN, 24);
        delay(500);

        // 5도 방향 이동
        softPwmWrite(SERVO_PIN, 5);
        delay(500);
    }

    softPwmStop(SERVO_PIN);
    delay(500);

    return 0;
}