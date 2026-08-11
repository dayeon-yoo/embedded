# Active Buzzer Control

## Overview

A basic GPIO output control example using an active buzzer
and the WiringPi library on Raspberry Pi.

The buzzer is repeatedly turned on and off at one-second
intervals.

## Hardware

- Raspberry Pi 3 Model B+
- Active Buzzer

## GPIO Mapping

| Component | WiringPi Pin |
|-----------|--------------|
| Active Buzzer | 30 |

## Implementation

- Initialize Raspberry Pi GPIO using WiringPi
- Configure the buzzer pin as `OUTPUT`
- Turn the buzzer ON using `digitalWrite(HIGH)`
- Turn the buzzer OFF using `digitalWrite(LOW)`
- Repeat the ON/OFF cycle every one second

## Build

```bash
gcc main.c -o buzzer_control -lwiringPi
```

## Run

```bash
sudo ./buzzer_control
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.