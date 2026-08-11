# DC Motor Control

## Overview

A basic DC motor control example using Raspberry Pi GPIO
and a BA6208 motor driver.

The motor repeatedly rotates in the counter-clockwise
direction, clockwise direction, and then stops.

Each state is maintained for one second.

## Hardware

- Raspberry Pi 3 Model B+
- DC Motor
- BA6208 Motor Driver

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Motor Input A | 6 |
| Motor Input B | 31 |

## Motor Control

The motor direction is controlled by changing the HIGH/LOW
combination of two GPIO output pins.

| Motor A | Motor B | State |
|---|---|---|
| HIGH | LOW | CCW |
| LOW | HIGH | CW |
| LOW | LOW | STOP |

## Implementation

1. Initialize WiringPi.
2. Configure the two motor control pins as `OUTPUT`.
3. Set the GPIO combination for counter-clockwise rotation.
4. Set the GPIO combination for clockwise rotation.
5. Set both pins to `LOW` to stop the motor.
6. Repeat the sequence continuously.

## Build

```bash
gcc main.c -o dc_motor -lwiringPi
```

## Run

```bash
sudo ./dc_motor
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.