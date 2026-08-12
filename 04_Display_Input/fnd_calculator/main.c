#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>

#define CLK_PIN 9
#define DIO_PIN 8
#define LED_PIN 27

#define KEY_INPUT 'q'
#define KEY_EXECUTE 'e'

/* TM1637 segment data */
static const uint8_t segmentMap[] =
{
    0x3f,   // 0
    0x06,   // 1
    0x5b,   // 2
    0x4f,   // 3
    0x66,   // 4
    0x6d,   // 5
    0x7d,   // 6
    0x07,   // 7
    0x7f,   // 8
    0x6f    // 9
};

/* Get keyboard input without pressing Enter */
int getch(void)
{
    struct termios oldattr;
    struct termios newattr;
    int ch;

    tcgetattr(STDIN_FILENO, &oldattr);

    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}

/* TM1637 start condition */
void tm1637Start(void)
{
    pinMode(DIO_PIN, OUTPUT);

    digitalWrite(DIO_PIN, HIGH);
    digitalWrite(CLK_PIN, HIGH);

    digitalWrite(DIO_PIN, LOW);
}

/* TM1637 stop condition */
void tm1637Stop(void)
{
    pinMode(DIO_PIN, OUTPUT);

    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIO_PIN, LOW);

    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(DIO_PIN, HIGH);
}

/* Send one byte to TM1637 */
void tm1637WriteByte(uint8_t data)
{
    int i;

    pinMode(DIO_PIN, OUTPUT);

    for (i = 0; i < 8; i++)
    {
        digitalWrite(CLK_PIN, LOW);

        if (data & 0x01)
            digitalWrite(DIO_PIN, HIGH);
        else
            digitalWrite(DIO_PIN, LOW);

        data >>= 1;

        digitalWrite(CLK_PIN, HIGH);
    }

    /* ACK */
    digitalWrite(CLK_PIN, LOW);

    pinMode(DIO_PIN, INPUT);
    digitalWrite(CLK_PIN, HIGH);

    pinMode(DIO_PIN, OUTPUT);
}

/* Display four digits on FND */
void displayNumber(int number)
{
    int digits[4];

    if (number < 0)
        number = 0;

    if (number > 9999)
        number = 9999;

    digits[0] = (number / 1000) % 10;
    digits[1] = (number / 100) % 10;
    digits[2] = (number / 10) % 10;
    digits[3] = number % 10;

    /* Data command */
    tm1637Start();
    tm1637WriteByte(0x40);
    tm1637Stop();

    /* Address command */
    tm1637Start();
    tm1637WriteByte(0xC0);

    tm1637WriteByte(segmentMap[digits[0]]);
    tm1637WriteByte(segmentMap[digits[1]]);
    tm1637WriteByte(segmentMap[digits[2]]);
    tm1637WriteByte(segmentMap[digits[3]]);

    tm1637Stop();

    /* Display ON + brightness */
    tm1637Start();
    tm1637WriteByte(0x88 | 0x07);
    tm1637Stop();
}

int main(void)
{
    int num1;
    int num2;
    int result = 0;
    int inputReady = 0;
    char key;

    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    pinMode(CLK_PIN, OUTPUT);
    pinMode(DIO_PIN, OUTPUT);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    displayNumber(0);

    printf("Press q to input two positive integers.\n");
    printf("Press e to decrease the result by 1.\n");

    while (1)
    {
        key = getch();

        /* q : input two positive integers */
        if (key == KEY_INPUT)
        {
            printf("\nEnter two positive integers: ");

            scanf("%d %d", &num1, &num2);

            /* Remove remaining newline */
            while (getchar() != '\n');

            result = num1 + num2;

            printf("Sum = %d\n", result);

            displayNumber(result);

            inputReady = 1;
        }

        /* e : decrease result */
        else if (key == KEY_EXECUTE)
        {
            /*
             * q를 이용해 숫자를 입력한 이후에만 동작
             */
            if (inputReady)
            {
                if (result > 0)
                {
                    result--;

                    printf("Result = %d\n", result);

                    displayNumber(result);

                    /*
                     * 결과가 0이 되면 LED를 1초 동안 켠다.
                     */
                    if (result == 0)
                    {
                        digitalWrite(LED_PIN, HIGH);
                        delay(1000);
                        digitalWrite(LED_PIN, LOW);
                    }
                }
            }
        }
    }

    return 0;
}