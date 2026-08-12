#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <softPwm.h>

// ---------------------- Bluetooth & RX Buffer ----------------------

int g_hBT = -1;
char rx_buf[BUFSIZ] = "";

// ---------------------- DC / Servo Pin Define ---------------------

// WiringPi pin numbers
#define DCMotorA    6
#define DCMotorB    31
#define SERVO_PIN   26

// ---------------------- DC / Servo State --------------------------

int g_MotorSpeed = 50;
int g_MotorDir   = 0;

// Servo PWM value
int g_ServoPwm = 15;

#define SERVO_STEP  1
#define SERVO_RANGE 200

// ---------------------- Function Prototypes -----------------------

void InitBT(void);
void hw_init(void);
void parserCmd(void);
void applyDCMotor(void);

// ==================================================================
// main
// ==================================================================

int main(void)
{
    unsigned int nNextTime;

    InitBT();
    hw_init();

    nNextTime = millis();

    while (1) {

        // Bluetooth RX polling
        if (millis() > nNextTime + 50) {

            int i = 0;

            memset(rx_buf, 0, sizeof(rx_buf));

            while (serialDataAvail(g_hBT) > 0) {

                rx_buf[i] = serialGetchar(g_hBT);

                // '#' marks the end of a command
                if (rx_buf[i] == '#') {

                    if (i + 1 < BUFSIZ)
                        rx_buf[i + 1] = '\0';
                    else
                        rx_buf[BUFSIZ - 1] = '\0';

                    printf("RX = [%s]\n", rx_buf);

                    // Commands must start with '@'
                    if (rx_buf[0] == '@') {
                        parserCmd();
                    }

                    break;
                }

                if (i < BUFSIZ - 1)
                    i++;
            }

            nNextTime = millis();
        }

        // Prevent excessive CPU usage
        delay(1);
    }

    return 0;
}

// ==================================================================
// InitBT
// Bluetooth serial port initialization
// ==================================================================

void InitBT(void)
{
    g_hBT = serialOpen("/dev/rfcomm0", 115200);

    if (g_hBT < 0) {

        fprintf(
            stderr,
            "Unable to open /dev/rfcomm0: %s\n",
            strerror(errno)
        );

        exit(1);
    }

    printf("Bluetooth init OK (/dev/rfcomm0)\n");
}

// ==================================================================
// hw_init
// GPIO and softPwm initialization
// ==================================================================

void hw_init(void)
{
    if (wiringPiSetup() == -1) {

        fprintf(
            stderr,
            "wiringPiSetup Failed\n"
        );

        exit(1);
    }

    // ---------------- DC Motor ----------------

    pinMode(DCMotorA, SOFT_PWM_OUTPUT);
    pinMode(DCMotorB, SOFT_PWM_OUTPUT);

    softPwmCreate(
        DCMotorA,
        0,
        100
    );

    softPwmCreate(
        DCMotorB,
        0,
        100
    );

    // ---------------- Servo Motor ----------------

    pinMode(
        SERVO_PIN,
        SOFT_PWM_OUTPUT
    );

    softPwmCreate(
        SERVO_PIN,
        0,
        SERVO_RANGE
    );

    // ---------------- Initial State ----------------

    g_MotorSpeed = 50;
    g_MotorDir = 0;

    applyDCMotor();

    // Initial servo position
    g_ServoPwm = 15;

    softPwmWrite(
        SERVO_PIN,
        g_ServoPwm
    );

    printf(
        "Hardware init done. DC speed=%d, ServoPWM=%d\n",
        g_MotorSpeed,
        g_ServoPwm
    );
}

// ==================================================================
// parserCmd
// Bluetooth command parser
//
// Android -> Raspberry Pi
//
// @SPD,# : Check current speed
// @MCW,# : DC motor clockwise
// @MCC,# : DC motor counter-clockwise
// @ACC,# : Increase speed by 10
// @DEC,# : Decrease speed by 10
// @STP,# : Stop motor
// @SVL,# : Move servo left
// @SVR,# : Move servo right
//
// Raspberry Pi -> Android
//
// @SPD,50# : Current speed = 50
// ==================================================================

void parserCmd(void)
{
    // ---------------- SPD ----------------

    if (!strncmp(&rx_buf[1], "SPD", 3)) {

        printf(
            "CMD SPD: speed=%d\n",
            g_MotorSpeed
        );

        serialPrintf(
            g_hBT,
            "@SPD,%d#",
            g_MotorSpeed
        );

        printf(
            "TX SPD: @SPD,%d#\n",
            g_MotorSpeed
        );
    }

    // ---------------- MCW ----------------

    else if (!strncmp(&rx_buf[1], "MCW", 3)) {

        printf(
            "CMD MCW: DC Motor CW\n"
        );

        g_MotorDir = 1;

        applyDCMotor();
    }

    // ---------------- MCC ----------------

    else if (!strncmp(&rx_buf[1], "MCC", 3)) {

        printf(
            "CMD MCC: DC Motor CCW\n"
        );

        g_MotorDir = -1;

        applyDCMotor();
    }

    // ---------------- ACC ----------------

    else if (!strncmp(&rx_buf[1], "ACC", 3)) {

        g_MotorSpeed += 10;

        if (g_MotorSpeed > 100)
            g_MotorSpeed = 100;

        printf(
            "CMD ACC: speed=%d\n",
            g_MotorSpeed
        );

        applyDCMotor();
    }

    // ---------------- DEC ----------------

    else if (!strncmp(&rx_buf[1], "DEC", 3)) {

        g_MotorSpeed -= 10;

        if (g_MotorSpeed < 0)
            g_MotorSpeed = 0;

        printf(
            "CMD DEC: speed=%d\n",
            g_MotorSpeed
        );

        applyDCMotor();
    }

    // ---------------- STP ----------------

    else if (!strncmp(&rx_buf[1], "STP", 3)) {

        printf(
            "CMD STP: DC Motor STOP (speed=%d)\n",
            g_MotorSpeed
        );

        g_MotorDir = 0;

        applyDCMotor();
    }

    // ---------------- SVL ----------------

    else if (!strncmp(&rx_buf[1], "SVL", 3)) {

        g_ServoPwm -= SERVO_STEP;

        if (g_ServoPwm < 0)
            g_ServoPwm += SERVO_RANGE;

        printf(
            "CMD SVL: ServoPWM=%d\n",
            g_ServoPwm
        );

        softPwmWrite(
            SERVO_PIN,
            g_ServoPwm
        );
    }

    // ---------------- SVR ----------------

    else if (!strncmp(&rx_buf[1], "SVR", 3)) {

        g_ServoPwm += SERVO_STEP;

        if (g_ServoPwm >= SERVO_RANGE)
            g_ServoPwm -= SERVO_RANGE;

        printf(
            "CMD SVR: ServoPWM=%d\n",
            g_ServoPwm
        );

        softPwmWrite(
            SERVO_PIN,
            g_ServoPwm
        );
    }

    // ---------------- Unknown ----------------

    else {

        printf(
            "Unknown CMD: %s\n",
            rx_buf
        );
    }
}

// ==================================================================
// applyDCMotor
// Apply current motor speed and direction
//
// g_MotorDir == 0 : STOP
// g_MotorDir >  0 : CLOCKWISE
// g_MotorDir <  0 : COUNTER-CLOCKWISE
// ==================================================================

void applyDCMotor(void)
{
    int duty = g_MotorSpeed;

    if (duty < 0)
        duty = 0;

    if (duty > 100)
        duty = 100;

    // STOP
    if (g_MotorDir == 0) {

        softPwmWrite(
            DCMotorA,
            0
        );

        softPwmWrite(
            DCMotorB,
            0
        );
    }

    // CLOCKWISE
    else if (g_MotorDir > 0) {

        softPwmWrite(
            DCMotorA,
            duty
        );

        softPwmWrite(
            DCMotorB,
            0
        );
    }

    // COUNTER-CLOCKWISE
    else {

        softPwmWrite(
            DCMotorA,
            0
        );

        softPwmWrite(
            DCMotorB,
            duty
        );
    }
}