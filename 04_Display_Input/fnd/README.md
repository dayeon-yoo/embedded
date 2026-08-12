# 4-Digit FND Control

## Overview

A 4-digit FND control example using
Raspberry Pi and the TM1637 display module.

The program displays sequential numeric values
on the four digits of the FND.

## Display

| Digit | Description |
| ----- | ----------- |
| 1st   | Display data |
| 2nd   | Display data |
| 3rd   | Display data |
| 4th   | Display data |

The display data is continuously updated using
values from 0 to 15.

## Hardware

- Raspberry Pi 3 Model B+
- TM1637 4-Digit FND

## GPIO Mapping

| Signal | WiringPi Pin |
| ------ | ------------: |
| CLK    | 9 |
| DIO    | 8 |

## Implementation

- Initialize the Raspberry Pi GPIO environment
- Configure the TM1637 communication pins
- Initialize the TM1637 display
- Set the FND brightness
- Prepare display values using a number table
- Update four display digits
- Continuously change the displayed values
- Use a reimplemented TM1637 driver for display communication

## Build

```bash
gcc -o fnd main.c TM1637.c -lwiringPi -fcommon
```

## Run

```bash
sudo ./fnd
```

## Example

```text
0 1 2 3
1 2 3 4
2 3 4 5
3 4 5 6
...
```

The displayed values are continuously updated
at 500 ms intervals.

## Source

The main application was reconstructed from the
4-digit FND example in the Embedded Systems laboratory
materials.

The original TM1637 driver source files were unavailable,
so `TM1637.c` and `TM1637.h` were reimplemented to provide
the required TM1637 communication functions.