#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>

#define BUZZER_PIN 1

int main(void)
{
    // wiringPi 초기화
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!");
        return -1;
    }

    // Soft Tone 출력 설정
    softToneCreate(BUZZER_PIN);

    while (1)
    {
        // 도
        softToneWrite(BUZZER_PIN, 262);
        delay(500);

        // 레
        softToneWrite(BUZZER_PIN, 294);
        delay(500);

        // 미
        softToneWrite(BUZZER_PIN, 330);
        delay(500);

        // 파
        softToneWrite(BUZZER_PIN, 349);
        delay(500);

        // 솔
        softToneWrite(BUZZER_PIN, 392);
        delay(500);

        // 라
        softToneWrite(BUZZER_PIN, 440);
        delay(500);

        // 시
        softToneWrite(BUZZER_PIN, 494);
        delay(500);

        // 높은 도
        softToneWrite(BUZZER_PIN, 523);
        delay(500);
    }

    softToneStop(BUZZER_PIN);

    return 0;
}