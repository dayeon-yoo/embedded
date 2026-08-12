#ifndef TM1637_H
#define TM1637_H

#define BRIGHTEST 7

void TM1637_init(int clkPin, int dioPin);
void TM1637_set(int brightness, int mode, int address);
void TM1637_display(int position, int value);

#endif
