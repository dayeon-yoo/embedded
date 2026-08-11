# Servo Motor Control

## Overview

A basic servo motor control example using Soft PWM
on Raspberry Pi.

The program changes the servo motor position by
applying different PWM values.

## Hardware

- Raspberry Pi 3 Model B+
- Servo Motor

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Servo Motor | 26 |

## Implementation

- Initialize WiringPi
- Configure the servo motor pin as a Soft PWM output
- Set the PWM range from 0 to 200
- Move the servo motor using different PWM values
- Maintain each position for 500 ms

The example applies the following PWM values:

```text
15 → 24 → 5
```

## Servo Motor Operation

The servo motor operates using a PWM signal with
a 20 ms period.

The pulse width determines the servo motor position.

## Build

```bash
gcc main.c -o servo_motor -lwiringPi
```

## Run

```bash
sudo ./servo_motor
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.