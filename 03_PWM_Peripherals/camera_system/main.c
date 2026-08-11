#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define ULTRASONIC_TRIG 4
#define ULTRASONIC_ECHO 5

#define BUZZER_PIN 30
#define RED_LED_PIN 29

#define CAMERA_FILE "captured.jpeg"

float measureDistance(void)
{
    int start_time = 0;
    int end_time = 0;
    float duration = 0;
    float distance = 0;

    digitalWrite(ULTRASONIC_TRIG, LOW);
    delay(50);

    digitalWrite(ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG, LOW);

    while (digitalRead(ULTRASONIC_ECHO) == LOW)
        start_time = micros();

    while (digitalRead(ULTRASONIC_ECHO) == HIGH)
        end_time = micros();

    duration = (float)(end_time - start_time);
    duration = duration / 1000000 / 2;

    distance = (340 * duration) * 100;

    return distance;
}

void buzzerWarning(void)
{
    int i;

    /*
     * Short buzzer cycle for 3 seconds
     */
    for (i = 0; i < 6; i++)
    {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(250);

        digitalWrite(BUZZER_PIN, LOW);
        delay(250);
    }
}

void takePicture(void)
{
    char cmd[256];

    digitalWrite(RED_LED_PIN, HIGH);

    sprintf(
        cmd,
        "sudo libcamera-jpeg --width 800 --height 800 -t 1 -o %s",
        CAMERA_FILE
    );

    printf("%s\n", cmd);

    system(cmd);

    printf("Take a Picture!\n");

    digitalWrite(RED_LED_PIN, LOW);
}

int main(void)
{
    float distance;

    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    pinMode(ULTRASONIC_TRIG, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    /*
     * Initial state:
     * Ultrasonic sensor ready
     * Buzzer OFF
     * Red LED OFF
     */
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);

    printf("Press Enter to start the system.\n");
    getchar();

    printf("System started.\n");

    while (1)
    {
        distance = measureDistance();

        printf("Distance : %3.0f cm\n", distance);

        if (distance <= 20)
        {
            printf("Object detected within 20 cm.\n");
            printf("Buzzer warning for 3 seconds.\n");

            buzzerWarning();

            /*
             * Check whether the object is still within 20 cm.
             */
            distance = measureDistance();

            printf("Distance after warning : %3.0f cm\n", distance);

            if (distance <= 20)
            {
                takePicture();
            }
            else
            {
                printf("Object moved away. No picture taken.\n");
            }
        }

        delay(100);
    }

    return 0;
}