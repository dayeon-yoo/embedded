#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define R_LED_PIN 27    // PWM 기능 핀

int main(void)
{
    // wiringPi 초기화
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!");
        return -1;
    }

    // 소프트 PWM으로 핀 초기화
    pinMode(R_LED_PIN, SOFT_PWM_OUTPUT);

    // 출력 범위 설정
    softPwmCreate(R_LED_PIN, 0, 255);

    // RGB LED 밝기 설정
    while (1)
    {
        for (int idx = 0; idx <= 255; idx += 5)
        {
            softPwmWrite(R_LED_PIN, idx);
            printf("Red LED ON ~%d\n", idx);
            delay(50);
        }
    }

    return 0;
}