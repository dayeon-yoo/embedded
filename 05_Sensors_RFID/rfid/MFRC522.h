#ifndef MFRC522_H
#define MFRC522_H

#define MI_OK 0

#define PICC_REQIDL 0x26
#define PICC_REQALL 0x52

#define PICC_AUTHENT1A 0x60
#define PICC_AUTHENT1B 0x61

void PcdReset(void);
void PcdAntennaOn(void);
void PcdAntennaOff(void);

unsigned char PcdRequest(
    unsigned char reqMode,
    unsigned char *TagType
);

unsigned char PcdAnticoll(
    unsigned char *serNum
);

unsigned char PcdSelect(
    unsigned char *serNum
);

unsigned char PcdAuthState(
    unsigned char authMode,
    unsigned char BlockAddr,
    unsigned char *Sectorkey,
    unsigned char *serNum
);

unsigned char PcdWrite(
    unsigned char BlockAddr,
    unsigned char *writeData
);

unsigned char PcdBakValue(
    unsigned char sourceBlockAddr,
    unsigned char goalBlockAddr
);

unsigned char PcdRead(
    unsigned char BlockAddr,
    unsigned char *recvData
);

void PcdHalt(void);

#endif