# Step Motor Keyboard Control

## Overview

A keyboard-based stepper motor control example using
Raspberry Pi GPIO.

The user enters a rotation angle, and the stepper motor
rotates according to the requested direction.

## Controls

| Input | Action |
|---|---|
| Positive angle | Clockwise rotation |
| Negative angle | Counter-clockwise rotation |
| `0` | Stop |
| `q` | Quit |

## LED Status

| Motor State | LED |
|---|---|
| Clockwise | Red |
| Counter-clockwise | Blue |
| Stop | OFF |

## Hardware

- Raspberry Pi 3 Model B+
- Stepper Motor
- Stepper Motor Driver
- RGB LED

## GPIO Mapping

### Stepper Motor

| Motor Phase | WiringPi Pin |
|---|---:|
| Blue | 22 |
| Pink | 23 |
| Yellow | 24 |
| Orange | 25 |

### RGB LED

| LED | WiringPi Pin |
|---|---:|
| Blue | 27 |
| Green | 28 |
| Red | 29 |

## Implementation

- Receive a rotation angle from keyboard input
- Determine the rotation direction from the sign of the angle
- Convert the requested angle into the corresponding number of steps
- Apply the four-phase stepper motor sequence
- Indicate the rotation direction using an RGB LED
- Turn the LED off when the motor stops

## Build

```bash
gcc main.c -o step_motor_keyboard -lwiringPi
```

## Run

```bash
sudo ./step_motor_keyboard
```

## Example

```text
Angle: 90
Motor CW: 90 degrees

Angle: -180
Motor CCW: 180 degrees

Angle: 0
Motor STOP
```

## Source

Reconstructed from the stepper motor keyboard-control
assignment requirements in the Embedded Systems laboratory
materials.