# LCD Control

## Overview

An LCD control example using Raspberry Pi and
PCF8574-based I2C communication.

The program initializes a 16x2 LCD and displays
two messages on separate lines.

## Display

| Line | Output        |
| ---- | ------------- |
| 1    | Hello, World! |
| 2    | Good Luck *   |

## Hardware

- Raspberry Pi 3 Model B+
- 16x2 LCD
- PCF8574 I2C Interface

## GPIO Mapping

| LCD Signal | WiringPi Pin |
| ---------- | -----------: |
| RS         |          100 |
| RW         |          101 |
| EN         |          102 |
| BL         |          103 |
| D4         |          104 |
| D5         |          105 |
| D6         |          106 |
| D7         |          107 |

## Implementation

- Initialize the Raspberry Pi GPIO environment
- Configure the PCF8574 interface
- Configure the LCD read/write control pin
- Control the LCD backlight
- Initialize a 16x2 LCD
- Clear the LCD screen
- Set the cursor position
- Display text on separate lines
- Repeat the display sequence continuously

## Build

```bash
gcc main.c -o lcd -lwiringPi -lwiringPiDev
```

## Run

```bash
sudo ./lcd
```

## Example

```bash
Hello, World!
Good Luck *
```

## Source

Reconstructed from the LCD example in the
Embedded Systems laboratory materials.