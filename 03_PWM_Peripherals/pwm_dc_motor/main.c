#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define DCMotorA 6
#define DCMotorB 31

int main(void)
{
    wiringPiSetup();

    pinMode(DCMotorA, SOFT_PWM_OUTPUT);
    pinMode(DCMotorB, SOFT_PWM_OUTPUT);

    softPwmCreate(DCMotorA, 0, 100);
    softPwmCreate(DCMotorB, 0, 100);

    int duty = 10;
    int updown = 10;

    while (1)
    {
        if (duty > 90)
            updown = -10;

        if (duty < 20)
            updown = 10;

        duty += updown;

        printf("Motor duty = %d\n", duty);

        // 반시계 방향 (CCW)
        softPwmWrite(DCMotorA, duty);
        softPwmWrite(DCMotorB, 0);
        printf("Motor run CCW\n");
        delay(3000);

        // 정지
        softPwmWrite(DCMotorA, 0);
        softPwmWrite(DCMotorB, 0);
        printf("Motor STOP\n");
        delay(1000);

        // 시계 방향 (CW)
        softPwmWrite(DCMotorA, 0);
        softPwmWrite(DCMotorB, duty);
        printf("Motor run CW\n");
        delay(3000);

        // 정지
        softPwmWrite(DCMotorA, 0);
        softPwmWrite(DCMotorB, 0);
        printf("Motor STOP\n");
        delay(1000);
    }

    return 0;
}