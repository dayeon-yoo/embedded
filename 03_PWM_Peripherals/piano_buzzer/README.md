# Piano Buzzer

## Overview

A keyboard-controlled piano example using a passive buzzer
and Soft Tone on Raspberry Pi.

Each keyboard key produces one of eight musical notes.
The `getch()` function is used so that a note is played
immediately without requiring the Enter key.

## Hardware

- Raspberry Pi 3 Model B+
- Passive Buzzer

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Passive Buzzer | 21 |

## Implementation

- Initialize WiringPi
- Create a Soft Tone output for the passive buzzer
- Receive keyboard input without pressing Enter
- Map keyboard keys to eight musical notes
- Output each note for 125 ms
- Stop the tone after each note

### Keyboard Mapping

| Key | Note | Frequency |
|---|---|---:|
| `a` | Low Do | 262 Hz |
| `s` | Re | 294 Hz |
| `d` | Mi | 330 Hz |
| `f` | Fa | 349 Hz |
| `g` | Sol | 392 Hz |
| `h` | La | 440 Hz |
| `j` | Si | 494 Hz |
| `k` | High Do | 523 Hz |
| `q` | Quit | - |

## Build

```bash
gcc main.c -o piano_buzzer -lwiringPi
```

## Run

```bash
sudo ./piano_buzzer
```

Press `q` to exit the program.

## Source

Reconstructed from the piano buzzer assignment requirements
in the Embedded Systems laboratory materials.

The assignment specifies eight notes and a 125 ms note duration.
The `a` through `k` keyboard mapping used in this implementation
is a reconstructed key mapping based on the eight-note requirement.