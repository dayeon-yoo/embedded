# Display and Input

## Overview

This section contains Raspberry Pi examples for
display devices and input devices.

The examples cover LCD, FND, IR remote control,
keyboard input, and combinations of these peripherals.

## Examples

| Directory | Description |
| --------- | ----------- |
| `lcd` | Display text using a 16x2 LCD |
| `fnd` | Display numbers using a 4-digit FND |
| `ir_remote` | Receive and process IR remote control signals |
| `fnd_calculator` | Perform a simple calculation and display the result on FND |
| `ir_lcd_control` | Control LCD and buzzer using an IR remote |

## Hardware

- Raspberry Pi 3 Model B+
- 16x2 LCD
- PCF8574 I2C Expander
- 4-Digit FND
- TM1637 Module
- IR Receiver
- IR Remote Controller
- Passive Buzzer
- LED

## Implemented Functions

### LCD

- Initialize a 16x2 LCD
- Clear the LCD screen
- Set the cursor position
- Display text messages
- Control the LCD backlight

### FND

- Initialize the TM1637 display
- Display numeric values
- Control individual digits
- Display calculation results

### IR Remote

- Receive IR remote control signals
- Read IR button codes
- Map button codes to specific actions
- Control peripherals using the remote

### FND Calculator

- Input two positive integers
- Calculate the sum of the two integers
- Display the result on the FND
- Decrease the displayed value by one
- Turn on an LED for one second when the value reaches zero

### IR LCD Control

- Control the door state using an IR remote
- Display `Open the door` on the LCD
- Display `Close the door` on the LCD
- Play an ascending melody when opening
- Play a descending melody when closing
- Ignore commands that do not match the current door state

## Directory Structure

    04_Display_Input/
    ├── README.md
    ├── lcd/
    │   ├── main.c
    │   └── README.md
    ├── fnd/
    │   ├── main.c
    │   └── README.md
    ├── ir_remote/
    │   ├── main.c
    │   └── README.md
    ├── fnd_calculator/
    │   ├── main.c
    │   └── README.md
    └── ir_lcd_control/
        ├── main.c
        └── README.md

## Source

Reconstructed from the Display and Input
laboratory materials in the Embedded Systems course.