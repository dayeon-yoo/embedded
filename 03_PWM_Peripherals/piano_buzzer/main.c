#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softTone.h>

#define BUZZER_PIN 21

#define NOTE_DO_LOW   262
#define NOTE_RE       294
#define NOTE_MI       330
#define NOTE_FA       349
#define NOTE_SOL      392
#define NOTE_LA       440
#define NOTE_SI       494
#define NOTE_DO_HIGH  523

char getch(void)
{
    char c;
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr);

    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    newattr.c_cc[VMIN] = 1;
    newattr.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return c;
}

void playNote(int frequency)
{
    softToneWrite(BUZZER_PIN, frequency);
    delay(125);

    softToneWrite(BUZZER_PIN, 0);
}

int main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    softToneCreate(BUZZER_PIN);

    printf("Piano Buzzer\n");
    printf("a s d f g h j k : Do Re Mi Fa Sol La Si Do\n");
    printf("q : Quit\n");

    while (1)
    {
        char key = getch();

        switch (key)
        {
            case 'a':
                playNote(NOTE_DO_LOW);
                break;

            case 's':
                playNote(NOTE_RE);
                break;

            case 'd':
                playNote(NOTE_MI);
                break;

            case 'f':
                playNote(NOTE_FA);
                break;

            case 'g':
                playNote(NOTE_SOL);
                break;

            case 'h':
                playNote(NOTE_LA);
                break;

            case 'j':
                playNote(NOTE_SI);
                break;

            case 'k':
                playNote(NOTE_DO_HIGH);
                break;

            case 'q':
                softToneWrite(BUZZER_PIN, 0);
                softToneStop(BUZZER_PIN);
                return 0;

            default:
                break;
        }
    }

    return 0;
}