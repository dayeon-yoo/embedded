#include <wiringPi.h>
#include "TM1637.h"

#define CMD_DATA_AUTO  0x40
#define CMD_DATA_FIXED 0x44
#define CMD_ADDR_BASE  0xC0
#define CMD_DISPLAY    0x88

static int clkPin;
static int dioPin;

static const unsigned char digitCode[] =
{
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
};

static void TM1637_start(void)
{
    pinMode(dioPin, OUTPUT);
    digitalWrite(dioPin, HIGH);

    digitalWrite(clkPin, HIGH);
    delayMicroseconds(2);

    digitalWrite(dioPin, LOW);
    delayMicroseconds(2);

    digitalWrite(clkPin, LOW);
}

static void TM1637_stop(void)
{
    pinMode(dioPin, OUTPUT);

    digitalWrite(clkPin, LOW);
    digitalWrite(dioPin, LOW);

    digitalWrite(clkPin, HIGH);
    delayMicroseconds(2);

    digitalWrite(dioPin, HIGH);
    delayMicroseconds(2);
}

static void TM1637_writeByte(unsigned char data)
{
    int i;

    for (i = 0; i < 8; i++)
    {
        digitalWrite(clkPin, LOW);

        if (data & 0x01)
            digitalWrite(dioPin, HIGH);
        else
            digitalWrite(dioPin, LOW);

        delayMicroseconds(3);

        digitalWrite(clkPin, HIGH);
        delayMicroseconds(3);

        data >>= 1;
    }

    /*
     * ACK bit
     */
    digitalWrite(clkPin, LOW);
    pinMode(dioPin, INPUT);

    delayMicroseconds(3);

    digitalWrite(clkPin, HIGH);
    delayMicroseconds(3);

    digitalWrite(clkPin, LOW);
    pinMode(dioPin, OUTPUT);
}

void TM1637_init(int clk, int dio)
{
    clkPin = clk;
    dioPin = dio;

    pinMode(clkPin, OUTPUT);
    pinMode(dioPin, OUTPUT);

    digitalWrite(clkPin, HIGH);
    digitalWrite(dioPin, HIGH);
}

void TM1637_set(int brightness, int mode, int address)
{
    /*
     * Data command
     */
    TM1637_start();
    TM1637_writeByte(mode);
    TM1637_stop();

    /*
     * Address command
     */
    TM1637_start();
    TM1637_writeByte(address);
    TM1637_stop();

    /*
     * Display control command
     *
     * Brightness range: 0 ~ 7
     * 0x88 = display ON, brightness 0
     */
    TM1637_start();

    if (brightness < 0)
        brightness = 0;

    if (brightness > 7)
        brightness = 7;

    TM1637_writeByte(0x88 | brightness);

    TM1637_stop();
}

void TM1637_display(int position, int value)
{
    if (position < 0 || position > 3)
        return;

    if (value < 0 || value > 15)
        return;

    TM1637_start();
    TM1637_writeByte(CMD_DATA_FIXED);
    TM1637_stop();

    TM1637_start();
    TM1637_writeByte(CMD_ADDR_BASE + position);
    TM1637_writeByte(digitCode[value]);
    TM1637_stop();

    /*
     * Keep display enabled.
     */
    TM1637_start();
    TM1637_writeByte(CMD_DISPLAY | BRIGHTEST);
    TM1637_stop();
}
