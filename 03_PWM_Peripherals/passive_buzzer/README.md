# Passive Buzzer Control

## Overview

A basic passive buzzer control example using Soft Tone
on Raspberry Pi.

The program generates eight musical notes by changing
the frequency applied to the passive buzzer.

## Hardware

- Raspberry Pi 3 Model B+
- Passive Buzzer

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Passive Buzzer | 1 |

## Implementation

- Initialize WiringPi
- Create a Soft Tone output on the buzzer pin
- Generate different frequencies for each musical note
- Maintain each note for 500 ms
- Repeat the musical scale continuously

### Musical Notes

| Note | Frequency |
|---|---:|
| Do | 262 Hz |
| Re | 294 Hz |
| Mi | 330 Hz |
| Fa | 349 Hz |
| Sol | 392 Hz |
| La | 440 Hz |
| Si | 494 Hz |
| High Do | 523 Hz |

## Build

```bash
gcc main.c -o passive_buzzer -lwiringPi
```

## Run

```bash
sudo ./passive_buzzer
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.