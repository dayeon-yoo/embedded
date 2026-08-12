# Bluetooth Buzzer Control

## Overview

A Bluetooth-based passive buzzer control example
using Raspberry Pi and WiringPi.

The Raspberry Pi receives commands from an Android
application through Bluetooth RFCOMM and controls
a passive buzzer.

The application supports seven basic notes,
Rabbit Song, Twinkle Twinkle Little Star,
and music stop control.

## Functions

| Command | Function |
| ------- | -------- |
| `@DO,#` | Play Do |
| `@RE,#` | Play Re |
| `@MI,#` | Play Mi |
| `@FA,#` | Play Fa |
| `@SOL,#` | Play Sol |
| `@LA,#` | Play La |
| `@SI,#` | Play Si |
| `@RABBIT,#` | Play Rabbit Song |
| `@STAR,#` | Play Twinkle Twinkle Little Star |
| `@STOP,#` | Stop music |

## Bluetooth Configuration

| Item | Setting |
| ---- | ------- |
| Device | `/dev/rfcomm0` |
| Baud Rate | `115200` |
| Communication | RFCOMM / SPP |

## Hardware

- Raspberry Pi 3 Model B+
- Passive Buzzer
- Bluetooth SPP connection

## GPIO Mapping

| Device | WiringPi Pin |
| ------ | ------------ |
| Passive Buzzer | 21 |

## Note Frequencies

| Note | Frequency |
| ---- | --------: |
| Do | 262 Hz |
| Re | 294 Hz |
| Mi | 330 Hz |
| Fa | 349 Hz |
| Sol | 392 Hz |
| La | 440 Hz |
| Si | 494 Hz |

## Implementation

- Initialize WiringPi
- Initialize the passive buzzer
- Open the Bluetooth RFCOMM device
- Receive commands from Android
- Detect `#` as the end of a command
- Play individual notes
- Play Rabbit Song
- Play Twinkle Twinkle Little Star
- Stop the current music
- Use a dedicated `PI_THREAD` for song playback

## Threaded Music Playback

Song playback is separated into a dedicated
`PI_THREAD`.

This allows the main loop to continue receiving
Bluetooth commands while a song is being played.

The playback thread checks the current song state
and stops playback when a stop request is received.

## Build

```bash
gcc -o buzzer_control main.c -lwiringPi
```

## Run

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./buzzer_control
```

## Example

```text
Bluetooth init OK (/dev/rfcomm0)

RX = [@DO,#]

RX = [@RABBIT,#]
CMD RABBIT: Play Rabbit Song

RX = [@STOP,#]
CMD STOP: Stop Music
```

## Source

Reconstructed from the Bluetooth buzzer-control
requirements in the Embedded Systems laboratory materials.