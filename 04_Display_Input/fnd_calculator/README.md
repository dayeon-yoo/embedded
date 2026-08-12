# FND Calculator

## Overview

A 4-digit FND calculation example using Raspberry Pi
and the TM1637 display module.

The user enters two positive integers using the keyboard.
The program displays the sum on the FND and decreases
the displayed value by one when the execute key is pressed.

## Controls

| Input | Action |
| ----- | ------ |
| `q` | Input two positive integers and display their sum |
| `e` | Decrease the displayed number by 1 |
| Other keys | No action |

## LED Status

| Condition | LED |
| --------- | --- |
| Result becomes `0` | ON for 1 second |
| Normal operation | OFF |

## Hardware

- Raspberry Pi 3 Model B+
- 4-Digit FND
- TM1637 Module
- LED

## GPIO Mapping

### TM1637 FND

| Signal | WiringPi Pin |
| ------ | ------------: |
| CLK | 9 |
| DIO | 8 |

### LED

| Device | WiringPi Pin |
| ------ | ------------: |
| LED | 27 |

## Implementation

- Press `q` to input two positive integers
- Calculate the sum of the two integers
- Display the result on the FND
- Press `e` to decrease the displayed number by 1
- Turn on the LED for 1 second when the value becomes `0`
- Ignore `e` when positive integers have not been entered using `q`
- Use keyboard input without pressing Enter for control keys

## Build

    gcc main.c -o fnd_calculator -lwiringPi

## Run

    sudo ./fnd_calculator

## Example

    Press q to input two positive integers.

    Enter two positive integers: 12 34
    Sum = 46

    Result = 45
    Result = 44
    Result = 43
    ...
    Result = 0

## Source

Reconstructed from the FND result display assignment
requirements in the Embedded Systems laboratory materials.