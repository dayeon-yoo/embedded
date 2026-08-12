# RFID Card Control

## Overview

An RFID card control example using Raspberry Pi,
SPI communication, and the MFRC522 RFID module.

The program detects an RFID card, identifies its card type,
reads the card serial number, authenticates a memory block,
writes data, backs up a value block, and reads card blocks.

## Hardware

- Raspberry Pi 3 Model B+
- MFRC522 RFID Reader
- RFID Card

## SPI Configuration

| Setting | Value |
| ------- | ----- |
| SPI Speed | 1 MHz |
| SPI CE Channel | CE0 |
| SPI Interface | SPI |

## Implementation

- Initialize WiringPi
- Initialize SPI communication
- Reset the MFRC522 RFID reader
- Control the RFID antenna
- Detect an RFID card
- Identify the card type
- Read the card serial number
- Select the detected card
- Authenticate Block 1 using the default key
- Write 16 bytes of data to Block 1
- Back up the value from Block 1 to Block 2
- Read Blocks 0 through 3
- Print the read data
- Halt the RFID card after processing

## RFID Data

### Default Key

| Byte | Value |
| ---- | ----- |
| 1 | `FF` |
| 2 | `FF` |
| 3 | `FF` |
| 4 | `FF` |
| 5 | `FF` |
| 6 | `FF` |

### Block 1 Data

`12 34 56 78 ED CB A9 87 12 34 56 78 01 FE 01 FE`

## Build

```bash
gcc -o rfid main.c MFRC522.c -lwiringPi
```

## Run

```bash
sudo ./rfid
```

## Example

```text
... Touch RFID CARD

card type:Mifare_One(S50)

card SN:
XXXXXXXX

read block 0:
...

read block 1:
...

read block 2:
...

read block 3:
...
```

## Source

Reconstructed from the RFID library and
MFRC522 laboratory example in the Embedded Systems course.

The MFRC522 driver is used as a separate source file
for RFID communication.