# Stepper Motor Control

## Overview

A stepper motor control example using Raspberry Pi GPIO.

The program controls a stepper motor by applying a
four-phase GPIO sequence. The motor rotates clockwise
and counter-clockwise and then enters a stop state.

## Hardware

- Raspberry Pi 3 Model B+
- Stepper Motor
- Stepper Motor Driver

## GPIO Mapping

| Motor Phase | WiringPi Pin |
|---|---:|
| Blue | 22 |
| Pink | 23 |
| Yellow | 24 |
| Orange | 25 |

## Implementation

The stepper motor is controlled by sequentially changing
the HIGH/LOW state of four GPIO pins.

### Clockwise

```text
HIGH HIGH LOW  LOW
LOW  HIGH HIGH LOW
LOW  LOW  HIGH HIGH
HIGH LOW  LOW  HIGH
```

### Counter-clockwise

The clockwise sequence is applied in reverse order.

```text
HIGH LOW  LOW  HIGH
LOW  LOW  HIGH HIGH
LOW  HIGH HIGH LOW
HIGH HIGH LOW  LOW
```

## Control Parameters

- 4 GPIO output pins
- 4-phase control sequence
- 512 sequence iterations
- 2 ms delay between each phase
- 500 ms stop interval

## Build

```bash
gcc main.c -o step_motor -lwiringPi
```

## Run

```bash
sudo ./step_motor
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.