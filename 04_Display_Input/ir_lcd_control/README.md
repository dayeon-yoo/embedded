# IR LCD Control

## Overview

An infrared remote control example using Raspberry Pi.

The program receives commands from an IR remote,
displays the door status on an LCD,
and plays a melody using a passive buzzer.

The initial state is closed and the LCD is blank.

## Controls

| Remote Button | Action |
| ------------- | ------ |
| Button 1 | Open the door |
| Button 2 | Close the door |

## LCD Output

| Action | LCD Message |
| ------ | ----------- |
| Open | Open the door |
| Close | Close the door |
| Initial state | Blank |

The message is displayed for approximately 1 second
and then disappears.

## Buzzer

| Action | Melody |
| ------ | ------ |
| Open | Do - Mi - Sol |
| Close | Sol - Mi - Do |

## Hardware

- Raspberry Pi 3 Model B+
- IR Receiver
- IR Remote Controller
- 16x2 LCD
- PCF8574 I2C Expander
- Passive Buzzer

## GPIO Mapping

### IR Receiver

| Device | WiringPi Pin |
| ------ | ------------: |
| IR Receiver | 18 |

### Passive Buzzer

| Device | WiringPi Pin |
| ------ | ------------: |
| Buzzer | 21 |

### LCD

| Signal | WiringPi Pin |
| ------ | ------------: |
| RS | 100 |
| RW | 101 |
| EN | 102 |
| Backlight | 103 |
| D4 | 104 |
| D5 | 105 |
| D6 | 106 |
| D7 | 107 |

## IR Codes

| Button | IR Code |
| ------ | ------- |
| Button 1 | 0x102a4f46 |
| Button 2 | 0x8534077e |

## Implementation

- Receive IR remote control signals
- Detect the remote button code
- Maintain the current door state
- Display "Open the door" when the door is opened
- Display "Close the door" when the door is closed
- Play an ascending melody when opening
- Play a descending melody when closing
- Ignore the open command when the door is already open
- Ignore the close command when the door is already closed
- Clear the LCD after displaying the message

## Build

    gcc main.c irpigpio.c -o ir_lcd_control -lpthread -lwiringPi -lpigpio

## Run

    sudo ./ir_lcd_control

## Example

    Button 1
    Open the door
    Do - Mi - Sol

    Button 2
    Close the door
    Sol - Mi - Do

## Source

Reconstructed from the IR remote and LCD control
assignment requirements in the Embedded Systems
laboratory materials.