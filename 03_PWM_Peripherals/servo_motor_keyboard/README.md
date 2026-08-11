# Servo Motor Keyboard Control

## Overview

A keyboard-controlled servo motor example using
Soft PWM and Raspberry Pi GPIO.

The servo motor position is changed immediately
according to keyboard input without requiring the
Enter key.

The LED brightness also changes according to the
servo motor angle.

## Hardware

- Raspberry Pi 3 Model B+
- Servo Motor
- LED

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Servo Motor | 26 |
| LED | 27 |

## Implementation

- Initialize WiringPi
- Configure the servo motor as a Soft PWM output
- Set the initial servo position to 15
- Limit the servo motor position from 6 to 24
- Receive keyboard input without pressing Enter
- Move the servo motor by one step for each key input
- Return the servo motor to the center position
- Adjust LED brightness according to the servo motor angle

### Keyboard Control

| Key | Action |
|---|---|
| `q` | Move counter-clockwise by 1 |
| `w` | Move to center position (15) |
| `e` | Move clockwise by 1 |

### Servo Range

```text
6 ←──────── 15 ────────→ 24
Minimum       Center       Maximum
```

The LED is turned off at the minimum angle
and becomes progressively brighter as the angle increases.

## Build

```bash
gcc main.c -o servo_motor_keyboard -lwiringPi
```

## Run

```bash
sudo ./servo_motor_keyboard
```

Press `Ctrl + C` to stop the program.

## Source

Reconstructed from the servo motor keyboard-control
assignment requirements and the `getch()` example
in the Embedded Systems laboratory materials.