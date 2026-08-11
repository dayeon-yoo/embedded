# PWM DC Motor Control

## Overview

A DC motor speed control example using Soft PWM
on Raspberry Pi.

The motor speed is controlled by changing the PWM
duty cycle, while the PWM output combination determines
the motor rotation direction.

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

- Initialize WiringPi
- Configure both motor control pins as Soft PWM outputs
- Set the PWM range from 0 to 100
- Gradually increase and decrease the duty cycle
- Control the motor in the counter-clockwise direction
- Stop the motor
- Control the motor in the clockwise direction
- Stop the motor again

The duty cycle is increased or decreased by 10
between 20 and 90.

### Motor Control

| Motor A | Motor B | State |
|---|---|---|
| Duty | 0 | CCW |
| 0 | 0 | STOP |
| 0 | Duty | CW |
| 0 | 0 | STOP |

## Build

```bash
gcc main.c -o pwm_dc_motor -lwiringPi
```

## Run

```bash
sudo ./pwm_dc_motor
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.