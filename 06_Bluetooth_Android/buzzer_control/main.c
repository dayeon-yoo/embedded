#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <softTone.h>

/* ---------------------- Bluetooth ---------------------- */

#define BT_DEVICE "/dev/rfcomm0"
#define BT_BAUDRATE 115200

int g_hBT = -1;

/* ---------------------- Passive Buzzer ---------------------- */

#define BUZ_P_PIN 21

/* ---------------------- RX Buffer ---------------------- */

char rx_buf[BUFSIZ] = "";

/* ---------------------- Music State ---------------------- */

volatile int g_PlaySong = 0;
volatile int g_StopSong = 0;

/*
 * 0 : no song
 * 1 : Rabbit Song (산토끼)
 * 2 : Twinkle Twinkle Little Star (작은별)
 */

#define SONG_NONE  0
#define SONG_RABBIT 1
#define SONG_STAR   2

/* ---------------------- Note Frequencies ---------------------- */

#define NOTE_DO  262
#define NOTE_RE  294
#define NOTE_MI  330
#define NOTE_FA  349
#define NOTE_SOL 392
#define NOTE_LA  440
#define NOTE_SI  494

/* ---------------------- Function Prototypes ---------------------- */

void InitBT(void);
void hw_init(void);
void parserCmd(void);

void playNote(
    int frequency,
    int durationMs
);

void playRabbitSong(void);
void playStarSong(void);

PI_THREAD(musicThread);

/* ============================================================
 * main
 * ============================================================ */

int main(void)
{
    unsigned int nNextTime;

    InitBT();
    hw_init();

    /*
     * 음악 재생 Thread 생성
     */
    if (piThreadCreate(musicThread) != 0) {

        fprintf(
            stderr,
            "Failed to create music thread\n"
        );

        return -1;
    }

    nNextTime = millis();

    while (1) {

        /*
         * Bluetooth 수신 확인
         */
        if (millis() > nNextTime + 50) {

            int i = 0;

            memset(
                rx_buf,
                0,
                sizeof(rx_buf)
            );

            while (serialDataAvail(g_hBT) > 0) {

                rx_buf[i] =
                    serialGetchar(g_hBT);

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
 * Passive buzzer initialization
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
     * Passive buzzer
     */
    pinMode(
        BUZ_P_PIN,
        SOFT_TONE_OUTPUT
    );

    if (softToneCreate(BUZ_P_PIN) < 0) {

        fprintf(
            stderr,
            "softToneCreate Failed\n"
        );

        exit(1);
    }

    /*
     * Initially stop buzzer
     */
    softToneWrite(
        BUZ_P_PIN,
        0
    );
}

/* ============================================================
 * parserCmd
 * Bluetooth command parser
 *
 * Android -> Raspberry Pi
 *
 * @DO,#      : Do
 * @RE,#      : Re
 * @MI,#      : Mi
 * @FA,#      : Fa
 * @SOL,#     : Sol
 * @LA,#      : La
 * @SI,#      : Si
 * @RABBIT,#  : Rabbit Song
 * @STAR,#    : Twinkle Twinkle Little Star
 * @STOP,#    : Stop music
 * ============================================================ */

void parserCmd(void)
{
    /*
     * ---------------- DO ----------------
     */

    if (!strncmp(
            &rx_buf[1],
            "DO",
            2
        )) {

        g_StopSong = 1;

        playNote(
            NOTE_DO,
            300
        );
    }

    /*
     * ---------------- RE ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "RE",
                 2
             )) {

        g_StopSong = 1;

        playNote(
            NOTE_RE,
            300
        );
    }

    /*
     * ---------------- MI ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "MI",
                 2
             )) {

        g_StopSong = 1;

        playNote(
            NOTE_MI,
            300
        );
    }

    /*
     * ---------------- FA ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "FA",
                 2
             )) {

        g_StopSong = 1;

        playNote(
            NOTE_FA,
            300
        );
    }

    /*
     * ---------------- SOL ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "SOL",
                 3
             )) {

        g_StopSong = 1;

        playNote(
            NOTE_SOL,
            300
        );
    }

    /*
     * ---------------- LA ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "LA",
                 2
             )) {

        g_StopSong = 1;

        playNote(
            NOTE_LA,
            300
        );
    }

    /*
     * ---------------- SI ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "SI",
                 2
             )) {

        g_StopSong = 1;

        playNote(
            NOTE_SI,
            300
        );
    }

    /*
     * ---------------- RABBIT SONG ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "RABBIT",
                 6
             )) {

        printf(
            "CMD RABBIT: Play Rabbit Song\n"
        );

        g_StopSong = 1;

        delay(10);

        g_StopSong = 0;
        g_PlaySong = SONG_RABBIT;
    }

    /*
     * ---------------- STAR SONG ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "STAR",
                 4
             )) {

        printf(
            "CMD STAR: Play Star Song\n"
        );

        g_StopSong = 1;

        delay(10);

        g_StopSong = 0;
        g_PlaySong = SONG_STAR;
    }

    /*
     * ---------------- STOP ----------------
     */

    else if (!strncmp(
                 &rx_buf[1],
                 "STOP",
                 4
             )) {

        printf(
            "CMD STOP: Stop Music\n"
        );

        g_StopSong = 1;
        g_PlaySong = SONG_NONE;

        softToneWrite(
            BUZ_P_PIN,
            0
        );
    }

    /*
     * ---------------- UNKNOWN ----------------
     */

    else {

        printf(
            "Unknown CMD: %s\n",
            rx_buf
        );
    }
}

/* ============================================================
 * playNote
 * Play a single note
 * ============================================================ */

void playNote(
    int frequency,
    int durationMs
)
{
    softToneWrite(
        BUZ_P_PIN,
        frequency
    );

    delay(
        durationMs
    );

    softToneWrite(
        BUZ_P_PIN,
        0
    );

    delay(50);
}

/* ============================================================
 * Rabbit Song
 * 산토끼
 * ============================================================ */

void playRabbitSong(void)
{
    /*
     * Simple melody sequence
     *
     * The sequence is represented using the
     * basic seven notes used in the assignment.
     */

    int melody[] = {
        NOTE_SOL, NOTE_MI, NOTE_MI,
        NOTE_SOL, NOTE_MI, NOTE_DO,
        NOTE_RE, NOTE_MI, NOTE_RE,
        NOTE_DO
    };

    int duration[] = {
        300, 300, 300,
        300, 300, 300,
        300, 300, 300,
        500
    };

    int count =
        sizeof(melody) / sizeof(melody[0]);

    int i;

    for (i = 0; i < count; i++) {

        if (g_StopSong)
            return;

        playNote(
            melody[i],
            duration[i]
        );
    }
}

/* ============================================================
 * Star Song
 * 작은별
 * ============================================================ */

void playStarSong(void)
{
    int melody[] = {
        NOTE_DO, NOTE_DO, NOTE_SOL,
        NOTE_SOL, NOTE_LA, NOTE_LA,
        NOTE_SOL,

        NOTE_FA, NOTE_FA, NOTE_MI,
        NOTE_MI, NOTE_RE, NOTE_RE,
        NOTE_DO
    };

    int duration[] = {
        300, 300, 300,
        300, 300, 300,
        500,

        300, 300, 300,
        300, 300, 300,
        500
    };

    int count =
        sizeof(melody) / sizeof(melody[0]);

    int i;

    for (i = 0; i < count; i++) {

        if (g_StopSong)
            return;

        playNote(
            melody[i],
            duration[i]
        );
    }
}

/* ============================================================
 * musicThread
 * Music playback thread
 * ============================================================ */

PI_THREAD(musicThread)
{
    while (1) {

        /*
         * Rabbit Song
         */
        if (g_PlaySong == SONG_RABBIT) {

            g_StopSong = 0;

            playRabbitSong();

            g_PlaySong = SONG_NONE;
        }

        /*
         * Star Song
         */
        else if (g_PlaySong == SONG_STAR) {

            g_StopSong = 0;

            playStarSong();

            g_PlaySong = SONG_NONE;
        }

        delay(10);
    }

    return NULL;
}