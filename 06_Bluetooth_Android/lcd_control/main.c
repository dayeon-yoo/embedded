#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <pcf8574.h>
#include <lcd.h>

/* ---------------------- Bluetooth ---------------------- */

#define BT_DEVICE "/dev/rfcomm0"
#define BT_BAUDRATE 115200

int g_hBT = -1;

/* ---------------------- LCD ---------------------- */

#define BASE_ADDR 100

#define RS_PIN BASE_ADDR
#define RW_PIN (BASE_ADDR + 1)
#define EN_PIN (BASE_ADDR + 2)
#define BL_PIN (BASE_ADDR + 3)

#define D4_PIN (BASE_ADDR + 4)
#define D5_PIN (BASE_ADDR + 5)
#define D6_PIN (BASE_ADDR + 6)
#define D7_PIN (BASE_ADDR + 7)

int lcd_fd;

/* ---------------------- RX Buffer ---------------------- */

char rx_buf[BUFSIZ] = "";

/* ---------------------- Function Prototypes ---------------------- */

void InitBT(void);
void hw_init(void);
void parserCmd(void);

/* ============================================================
 * main
 * ============================================================ */

int main(void)
{
    unsigned int nNextTime;

    InitBT();
    hw_init();

    nNextTime = millis();

    while (1) {

        /*
         * Bluetooth 수신 확인
         * 약 50ms 간격으로 확인
         */
        if (millis() > nNextTime + 50) {

            int i = 0;

            memset(
                rx_buf,
                0,
                sizeof(rx_buf)
            );

            while (serialDataAvail(g_hBT) > 0) {

                rx_buf[i] = serialGetchar(g_hBT);

                /*
                 * '#'을 명령의 끝으로 사용
                 */
                if (rx_buf[i] == '#') {

                    if (i + 1 < BUFSIZ)
                        rx_buf[i + 1] = '\0';
                    else
                        rx_buf[BUFSIZ - 1] = '\0';

                    printf(
                        "RX = [%s]\n",
                        rx_buf
                    );

                    /*
                     * '@'로 시작하는 명령만 처리
                     */
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

        delay(1);
    }

    return 0;
}

/* ============================================================
 * InitBT
 * Bluetooth initialization
 * ============================================================ */

void InitBT(void)
{
    g_hBT = serialOpen(
        BT_DEVICE,
        BT_BAUDRATE
    );

    if (g_hBT < 0) {

        fprintf(
            stderr,
            "Unable to open /dev/rfcomm0: %s\n",
            strerror(errno)
        );

        exit(1);
    }

    printf(
        "Bluetooth init OK (%s)\n",
        BT_DEVICE
    );
}

/* ============================================================
 * hw_init
 * LCD and GPIO initialization
 * ============================================================ */

void hw_init(void)
{
    if (wiringPiSetup() == -1) {

        fprintf(
            stderr,
            "wiringPiSetup Failed\n"
        );

        exit(1);
    }

    /*
     * PCF8574 I2C 설정
     *
     * LCD에 연결된 PCF8574의 주소는 0x27
     */
    pcf8574Setup(
        RS_PIN,
        0x27
    );

    /*
     * RW는 항상 쓰기 모드
     */
    pinMode(
        RW_PIN,
        OUTPUT
    );

    digitalWrite(
        RW_PIN,
        LOW
    );

    /*
     * 백라이트 초기 상태: ON
     */
    pinMode(
        BL_PIN,
        OUTPUT
    );

    digitalWrite(
        BL_PIN,
        HIGH
    );

    /*
     * 16x2 LCD 초기화
     *
     * 4-bit mode
     */
    lcd_fd = lcdInit(
        2,
        16,
        4,
        RS_PIN,
        EN_PIN,
        D4_PIN,
        D5_PIN,
        D6_PIN,
        D7_PIN,
        0,
        0,
        0,
        0
    );

    if (lcd_fd < 0) {

        printf(
            "lcdInit failed\n"
        );

        exit(1);
    }

    /*
     * 초기 LCD는 비워둔다.
     */
    lcdClear(lcd_fd);

    printf(
        "LCD initialization complete\n"
    );
}

/* ============================================================
 * parserCmd
 * Bluetooth command parser
 *
 * Android -> Raspberry Pi
 *
 * @LCD,text# : LCD에 텍스트 출력
 * @LON,#     : LCD Backlight ON
 * @LOFF,#    : LCD Backlight OFF
 * @LCLR,#    : LCD Clear
 * ============================================================ */

void parserCmd(void)
{
    /*
     * --------------------------------------------------------
     * LCD Text
     * --------------------------------------------------------
     *
     * 형식:
     *
     * @LCD,Hello#
     *
     * 자료의 p3Activity 예제에서도
     * sendMessage("@LCD," + lcdtxt + "#")
     * 형식을 사용한다.
     */
    if (!strncmp(
            &rx_buf[1],
            "LCD,",
            4
        )) {

        char *text;

        /*
         * '@LCD,' 뒤의 텍스트를 가져온다.
         */
        text = &rx_buf[5];

        /*
         * 마지막 '#' 제거
         */
        char *end = strchr(
            text,
            '#'
        );

        if (end != NULL) {
            *end = '\0';
        }

        /*
         * LCD는 16자 이내로 제한
         */
        text[16] = '\0';

        printf(
            "CMD LCD: %s\n",
            text
        );

        lcdClear(lcd_fd);

        lcdPosition(
            lcd_fd,
            0,
            0
        );

        lcdPuts(
            lcd_fd,
            text
        );
    }

    /*
     * --------------------------------------------------------
     * LCD Backlight ON
     * --------------------------------------------------------
     */
    else if (!strncmp(
                 &rx_buf[1],
                 "LON",
                 3
             )) {

        printf(
            "CMD LON: LCD Backlight ON\n"
        );

        digitalWrite(
            BL_PIN,
            HIGH
        );
    }

    /*
     * --------------------------------------------------------
     * LCD Backlight OFF
     * --------------------------------------------------------
     */
    else if (!strncmp(
                 &rx_buf[1],
                 "LOFF",
                 4
             )) {

        printf(
            "CMD LOFF: LCD Backlight OFF\n"
        );

        digitalWrite(
            BL_PIN,
            LOW
        );
    }

    /*
     * --------------------------------------------------------
     * LCD Clear
     * --------------------------------------------------------
     */
    else if (!strncmp(
                 &rx_buf[1],
                 "LCLR",
                 4
             )) {

        printf(
            "CMD LCLR: LCD Clear\n"
        );

        lcdClear(lcd_fd);
    }

    /*
     * --------------------------------------------------------
     * Unknown command
     * --------------------------------------------------------
     */
    else {

        printf(
            "Unknown CMD: %s\n",
            rx_buf
        );
    }
}