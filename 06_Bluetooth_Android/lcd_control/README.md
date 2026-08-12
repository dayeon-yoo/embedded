# Bluetooth LCD Control

## Overview

A Bluetooth-based Text LCD control example using
Raspberry Pi, PCF8574, and WiringPi.

The Raspberry Pi receives commands from an Android
application through Bluetooth RFCOMM and controls
a 16x2 Text LCD.

## Functions

| Command | Function |
| ------- | -------- |
| `@LCD,text#` | Display text on the LCD |
| `@LON,#` | Turn LCD backlight ON |
| `@LOFF,#` | Turn LCD backlight OFF |
| `@LCLR,#` | Clear LCD text |

## Bluetooth Configuration

| Item | Setting |
| ---- | ------- |
| Device | `/dev/rfcomm0` |
| Baud Rate | `115200` |
| Communication | RFCOMM / SPP |

## Hardware

- Raspberry Pi 3 Model B+
- 16x2 Text LCD
- PCF8574 I2C module
- Bluetooth SPP connection

## LCD Configuration

| Setting | Value |
| ------- | ----- |
| PCF8574 Address | `0x27` |
| LCD Mode | 4-bit |
| LCD Size | 16x2 |

## GPIO Mapping

| LCD Signal | WiringPi Pin |
| ---------- | ------------ |
| RS | 100 |
| RW | 101 |
| EN | 102 |
| Backlight | 103 |
| D4 | 104 |
| D5 | 105 |
| D6 | 106 |
| D7 | 107 |

The LCD signals are mapped through the PCF8574
starting from the base address 100.

## Text Input

The Android application sends up to 16 characters
to the Raspberry Pi.

Example:

```text
@LCD,Hello#
```

The Raspberry Pi extracts the text after `@LCD,`
and displays it on the LCD.

## Implementation

- Initialize WiringPi
- Initialize the PCF8574 I2C interface
- Initialize the 16x2 Text LCD
- Open the Bluetooth RFCOMM device
- Receive commands from Android
- Detect `#` as the end of a command
- Display received text on the LCD
- Control the LCD backlight
- Clear the LCD display

## Build

```bash
gcc -o lcd_control main.c -lwiringPi
```

## Run

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./lcd_control
```

## Example

```text
Bluetooth init OK (/dev/rfcomm0)
LCD initialization complete

RX = [@LCD,Hello#]
CMD LCD: Hello

RX = [@LON,#]
CMD LON: LCD Backlight ON

RX = [@LOFF,#]
CMD LOFF: LCD Backlight OFF

RX = [@LCLR,#]
CMD LCLR: LCD Clear
```

## Source

Reconstructed from the Bluetooth LCD control
requirements in the Embedded Systems laboratory materials.