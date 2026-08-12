#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "MFRC522.h"

#define SPI_SPEED 1000000
#define SPI_CE0_CHANNEL 0

#define CARD_BLOCK 1
#define BALANCE_INIT 10000

#define MAX_BALANCE 1000000

/*
 * RFID 교통카드
 *
 * Block 1의 앞 4바이트를 잔액 저장 공간으로 사용한다.
 *
 * balance:
 *   byte 0 : 잔액 상위 바이트
 *   byte 1
 *   byte 2
 *   byte 3 : 잔액 하위 바이트
 *
 * 나머지 바이트는 0으로 유지한다.
 */

unsigned char DefaultKey[6] =
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned char g_ucTempbuf[20];


/* ----------------------------------------
 * 키보드 입력 처리
 * ---------------------------------------- */

int getFunction(void)
{
    int function;

    printf("\nSelect Functions! (1.recharge  2.payment  3.check)\n");

    if (scanf("%d", &function) != 1) {
        while (getchar() != '\n');
        return -1;
    }

    return function;
}


int getAmount(const char *message)
{
    int amount;

    printf("%s", message);

    if (scanf("%d", &amount) != 1) {
        while (getchar() != '\n');
        return -1;
    }

    return amount;
}


/* ----------------------------------------
 * 잔액 변환 함수
 * ---------------------------------------- */

void balanceToData(int balance, unsigned char *data)
{
    memset(data, 0, 16);

    data[0] = (balance >> 24) & 0xFF;
    data[1] = (balance >> 16) & 0xFF;
    data[2] = (balance >> 8) & 0xFF;
    data[3] = balance & 0xFF;
}


int dataToBalance(unsigned char *data)
{
    int balance;

    balance = ((int)data[0] << 24)
            | ((int)data[1] << 16)
            | ((int)data[2] << 8)
            | (int)data[3];

    return balance;
}


/* ----------------------------------------
 * RFID 카드 초기화
 * ---------------------------------------- */

int rfidSetup(void)
{
    if (wiringPiSetup() == -1) {
        printf("Setup Fail ~!!\n");
        return -1;
    }

    if (wiringPiSPISetup(SPI_CE0_CHANNEL, SPI_SPEED) == -1) {
        fprintf(stderr,
                "wiringPiSPISetup Failed! ERROR: %s\n",
                strerror(errno));
        return -1;
    }

    delay(500);

    PcdReset();
    PcdAntennaOff();

    delay(10);

    PcdAntennaOn();

    return 0;
}


/* ----------------------------------------
 * RFID 카드 접근
 * ---------------------------------------- */

int cardConnect(void)
{
    unsigned char status;

    /*
     * 카드 탐색
     */
    status = PcdRequest(PICC_REQALL, g_ucTempbuf);

    if (status != MI_OK) {
        return -1;
    }

    /*
     * 카드 고유 번호 읽기
     */
    status = PcdAnticoll(g_ucTempbuf);

    if (status != MI_OK) {
        return -1;
    }

    /*
     * 카드 선택
     */
    status = PcdSelect(g_ucTempbuf);

    if (status != MI_OK) {
        return -1;
    }

    /*
     * Block 1 인증
     */
    status = PcdAuthState(
        PICC_AUTHENT1A,
        CARD_BLOCK,
        DefaultKey,
        g_ucTempbuf
    );

    if (status != MI_OK) {
        return -1;
    }

    return 0;
}


/* ----------------------------------------
 * 카드에서 잔액 읽기
 * ---------------------------------------- */

int readBalance(int *balance)
{
    unsigned char data[16];
    unsigned char status;

    status = PcdRead(CARD_BLOCK, data);

    if (status != MI_OK) {
        return -1;
    }

    *balance = dataToBalance(data);

    return 0;
}


/* ----------------------------------------
 * 카드에 잔액 저장
 * ---------------------------------------- */

int writeBalance(int balance)
{
    unsigned char data[16];
    unsigned char status;

    balanceToData(balance, data);

    status = PcdWrite(CARD_BLOCK, data);

    if (status != MI_OK) {
        return -1;
    }

    return 0;
}


/* ----------------------------------------
 * 카드 사용 종료
 * ---------------------------------------- */

void cardDisconnect(void)
{
    PcdHalt();
    delay(500);
}


/* ----------------------------------------
 * Recharge
 * ---------------------------------------- */

int recharge(void)
{
    int amount;
    int balance;

    printf("Function 1 is selected\n");

    amount = getAmount("Input the recharge amount\n");

    if (amount <= 0 || amount > MAX_BALANCE) {
        printf("Wrong Input\n");
        return -1;
    }

    printf("... Touch RFID CARD\n");

    if (cardConnect() < 0) {
        printf("Connection Error!\n");
        return -1;
    }

    /*
     * 현재 잔액 읽기
     */
    if (readBalance(&balance) < 0) {
        printf("Connection Error!\n");
        cardDisconnect();
        return -1;
    }

    /*
     * 처음 카드를 사용하는 경우
     * 잔액이 유효하지 않으면 10,000원으로 초기화
     */
    if (balance < 0 || balance > MAX_BALANCE) {
        balance = BALANCE_INIT;
    }

    /*
     * 충전
     */
    if (balance > MAX_BALANCE - amount) {
        printf("Balance Overflow\n");
        cardDisconnect();
        return -1;
    }

    balance += amount;

    /*
     * 카드에 저장
     */
    if (writeBalance(balance) < 0) {
        printf("Connection Error!\n");
        cardDisconnect();
        return -1;
    }

    printf("balance: %d\n", balance);

    cardDisconnect();

    return 0;
}


/* ----------------------------------------
 * Payment
 * ---------------------------------------- */

int payment(void)
{
    int amount;
    int balance;

    printf("Function 2 is selected\n");

    amount = getAmount("Input the payment amount\n");

    if (amount <= 0) {
        printf("Wrong Input\n");
        return -1;
    }

    printf("... Touch RFID CARD\n");

    if (cardConnect() < 0) {
        printf("Connection Error!\n");
        return -1;
    }

    /*
     * 현재 잔액 읽기
     */
    if (readBalance(&balance) < 0) {
        printf("Connection Error!\n");
        cardDisconnect();
        return -1;
    }

    /*
     * 잔액 부족
     */
    if (amount > balance) {
        printf("Not Enough balance\n");
        printf("balance: %d\n", balance);

        cardDisconnect();

        return 0;
    }

    /*
     * 결제 금액 차감
     */
    balance -= amount;

    /*
     * 카드에 저장
     */
    if (writeBalance(balance) < 0) {
        printf("Connection Error!\n");
        cardDisconnect();
        return -1;
    }

    printf("balance: %d\n", balance);

    cardDisconnect();

    return 0;
}


/* ----------------------------------------
 * Check
 * ---------------------------------------- */

int checkBalance(void)
{
    int balance;

    printf("Function 3 is selected\n");

    printf("... Touch RFID CARD\n");

    if (cardConnect() < 0) {
        printf("Connection Error!\n");
        return -1;
    }

    /*
     * 카드에서 잔액 읽기
     */
    if (readBalance(&balance) < 0) {
        printf("Connection Error!\n");
        cardDisconnect();
        return -1;
    }

    /*
     * 처음 check 하는 카드라면
     * 잔액을 10,000원으로 초기화
     */
    if (balance < 0 || balance > MAX_BALANCE) {
        balance = BALANCE_INIT;

        if (writeBalance(balance) < 0) {
            printf("Connection Error!\n");
            cardDisconnect();
            return -1;
        }
    }

    printf("balance: %d\n", balance);

    cardDisconnect();

    return 0;
}


/* ----------------------------------------
 * Main
 * ---------------------------------------- */

int main(void)
{
    int function;

    if (rfidSetup() < 0) {
        return -1;
    }

    printf("... Touch RFID CARD\n\n");

    while (1) {

        function = getFunction();

        switch (function) {

            case 1:
                recharge();
                break;

            case 2:
                payment();
                break;

            case 3:
                checkBalance();
                break;

            default:
                printf("Wrong Function\n");
                break;
        }
    }

    return 0;
}