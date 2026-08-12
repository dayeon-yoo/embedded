# P5 Activity - Bluetooth Motor Control

## Overview

A Bluetooth-based motor control example using
Raspberry Pi and WiringPi.

The Raspberry Pi receives commands from an Android
device through Bluetooth RFCOMM and controls a
DC motor and a servo motor.

## Bluetooth Configuration

| Item | Setting |
| ---- | ------- |
| Device | `/dev/rfcomm0` |
| Baud Rate | `115200` |
| Communication | RFCOMM / SPP |

## Hardware

- Raspberry Pi 3 Model B+
- DC Motor
- Servo Motor
- Bluetooth SPP connection

## GPIO Mapping

| Device | WiringPi Pin |
| ------ | ------------ |
| DC Motor A | 6 |
| DC Motor B | 31 |
| Servo Motor | 26 |

## DC Motor Control

| Command | Action |
| ------- | ------ |
| `@SPD,#` | Check current speed |
| `@MCW,#` | Clockwise rotation |
| `@MCC,#` | Counter-clockwise rotation |
| `@ACC,#` | Increase speed by 10 |
| `@DEC,#` | Decrease speed by 10 |
| `@STP,#` | Stop motor |

The motor speed is controlled from 0 to 100.

The initial speed is 50.

## Servo Motor Control

| Command | Action |
| ------- | ------ |
| `@SVL,#` | Move servo left |
| `@SVR,#` | Move servo right |

The servo uses a soft PWM range of 0 to 199.

The initial PWM value is 15.

When the PWM value reaches either end of
the range, it wraps around to the opposite end.

## Command Format

Bluetooth commands start with `@`
and end with `#`.

Example:

```text
@MCW,#
```

The Raspberry Pi checks the command after
receiving the `#` character.

## Response

When the Android device requests the current
motor speed using `@SPD,#`, the Raspberry Pi
responds with:

```text
@SPD,50#
```

The value represents the current motor speed.

## Implementation

- Initialize the Bluetooth RFCOMM device
- Initialize WiringPi
- Configure DC motor soft PWM
- Configure servo motor soft PWM
- Receive Bluetooth commands
- Parse commands beginning with `@`
- Detect `#` as the end of a command
- Control DC motor direction and speed
- Control servo motor position
- Send the current motor speed to Android

## Build

```bash
gcc -o p5Activity main.c -lwiringPi
```

## Run

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./p5Activity
```

## Example

```text
Bluetooth init OK (/dev/rfcomm0)
Hardware init done. DC speed=50, ServoPWM=15

RX = [@MCW,#]
CMD MCW: DC Motor CW

RX = [@ACC,#]
CMD ACC: speed=60

RX = [@SPD,#]
CMD SPD: speed=60
TX SPD: @SPD,60#

RX = [@STP,#]
CMD STP: DC Motor STOP (speed=60)
```

## Source

Reconstructed from the Bluetooth motor-control
example and P5 Activity requirements in the
Embedded Systems laboratory materials.