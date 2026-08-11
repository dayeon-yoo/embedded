# PWM LED Control

## Overview

A basic LED brightness control example using Soft PWM
on Raspberry Pi.

The program gradually increases the PWM value applied
to the LED, changing its brightness step by step.

## Hardware

- Raspberry Pi 3 Model B+
- LED
- Resistor

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Red LED | 27 |

## Implementation

- Initialize WiringPi
- Configure the LED pin as a Soft PWM output
- Set the PWM range from 0 to 255
- Gradually increase the PWM value from 0 to 255
- Control the LED brightness using `softPwmWrite()`
- Apply a short delay between each brightness step

The PWM value is increased by 5 at each step.

## Build

```bash
gcc main.c -o pwm_led -lwiringPi
```

## Run

```bash
sudo ./pwm_led
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.