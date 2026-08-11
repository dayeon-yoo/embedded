# RGB LED Control

## Overview

A basic GPIO output control example using an RGB LED
and the WiringPi library on Raspberry Pi.

The program sequentially controls the blue, green, and red
LED channels and then turns all channels off.

## Hardware

- Raspberry Pi 3 Model B+
- RGB LED

## GPIO Mapping

| Component | WiringPi Pin |
|-----------|--------------|
| Blue LED  | 27           |
| Green LED | 28           |
| Red LED   | 29           |

## Implementation

- Initialize Raspberry Pi GPIO using WiringPi
- Configure RGB LED pins as OUTPUT
- Control each LED channel using `digitalWrite()`
- Use `delay()` to control the duration of each state

## Build

```bash
gcc main.c -o rgb_led -lwiringPi
```

## Run

```bash
sudo ./rgb_led
```

Press `Ctrl + C` to stop the program.

## Source

Reconstructed from Embedded Systems laboratory lecture material.