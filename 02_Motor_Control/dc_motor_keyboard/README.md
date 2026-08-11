# DC Motor Keyboard Control

## Overview

A keyboard-controlled DC motor example using Raspberry Pi GPIO.

The program receives keyboard input without requiring the
Enter key and changes the motor direction or stops the motor
according to the input.

## Controls

| Key | Action |
|---|---|
| `+` | Clockwise rotation |
| `-` | Counter-clockwise rotation |
| `0` | Stop |
| `q` | Quit |

## Hardware

- Raspberry Pi 3 Model B+
- DC Motor
- BA6208 Motor Driver

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Motor Input A | 6 |
| Motor Input B | 31 |

## Implementation

- Configure the DC motor control pins as GPIO outputs
- Receive keyboard input without pressing Enter
- Interpret each key as a motor control command
- Control the motor direction using GPIO HIGH/LOW combinations
- Provide an explicit stop command
- Restore the terminal configuration after each key input

## Keyboard Input

The program uses the Linux `termios` interface to temporarily
disable canonical input mode and echo, allowing a single key
press to be processed immediately.

## Build

```bash
gcc main.c -o dc_motor_keyboard -lwiringPi
```

## Run

```bash
sudo ./dc_motor_keyboard
```

Press `q` to exit the program.

## Source

Reconstructed from the DC motor keyboard-control assignment
and the keyboard input example from the Embedded Systems
laboratory materials.