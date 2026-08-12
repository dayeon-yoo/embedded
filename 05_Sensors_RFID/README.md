# 05 Sensors & RFID

## Overview

This section contains Raspberry Pi sensor and RFID
experiments using WiringPi, SPI, MCP3208, and the MFRC522
RFID reader.

The examples cover analog sensor input, RFID card access,
RFID-based balance management, and a sensor-based safe
system using a Raspberry Pi camera.

## Contents

| Directory | Description |
| --------- | ----------- |
| `rfid` | Basic RFID card detection and data read/write |
| `rfid_traffic_card` | RFID-based traffic card system |
| `gold_mine` | Safe system using potentiometer, CDS, buzzer, LED, and camera |

## Hardware

- Raspberry Pi 3 Model B+
- MFRC522 RFID Reader
- MIFARE RFID Card
- MCP3208 ADC
- Potentiometer
- CDS Sensor
- Passive Buzzer
- RGB LED
- Raspberry Pi Camera

## Technologies

- C
- WiringPi
- SPI
- MCP3208 ADC
- MFRC522 RFID
- Raspberry Pi Camera

## 1. RFID

The `rfid` example demonstrates basic communication
between Raspberry Pi and the MFRC522 RFID reader.

The program detects an RFID card, reads the card type and
card serial number, authenticates the card, writes data to
Block 1, and reads card blocks.

### Main Features

- Initialize WiringPi and SPI
- Detect an RFID card
- Identify the card type
- Read the card serial number
- Select and authenticate the card
- Write data to Block 1
- Read RFID card blocks
- Display the card data

### Build

```bash
gcc -Wall -o rfid main.c MFRC522.c -lwiringPi
```

### Run

```bash
sudo ./rfid
```

## 2. RFID Traffic Card

The `rfid_traffic_card` example implements a simple
traffic card system using an RFID card.

Only Block 1 of the RFID card is used to store the balance.

### Functions

| Input | Function |
| ----- | -------- |
| `1` | Recharge |
| `2` | Payment |
| `3` | Check balance |

### Recharge

Adds the input amount to the balance stored on the RFID card.

### Payment

Subtracts the input amount from the RFID card balance.

If the payment amount is greater than the current balance,
the payment is cancelled and `Not Enough balance` is displayed.

### Check

Displays the current balance.

When the card is checked for the first time, the balance is
initialized to 10,000.

### RFID Block

| Data | Location |
| ---- | -------- |
| Balance | Block 1, Byte 0~3 |
| Unused | Block 1, Byte 4~15 |

### Build

```bash
gcc -Wall -o rfid_traffic_card main.c ../rfid/MFRC522.c -lwiringPi
```

### Run

```bash
sudo ./rfid_traffic_card
```

## 3. Gold Mine Safe

The `gold_mine` example implements a safe system using a
potentiometer, CDS sensor, passive buzzer, red LED, and
Raspberry Pi camera.

A random key value is generated after pressing Enter.
The potentiometer is then used to find the generated key.

### Operation

1. Wait for the Enter key.
2. Generate a random key value between 10 and 1000.
3. Read the potentiometer value periodically.
4. Turn on the red LED when the potentiometer value is
   within ±100 of the key value.
5. Open the safe when the potentiometer value is within ±10
   of the key value.
6. Play an ascending scale using the passive buzzer.
7. Check the CDS sensor after the safe is opened.
8. If the light level is low, take a picture using the
   Raspberry Pi camera.
9. Terminate the program.

### Sensor and Output Mapping

| Device | Channel / WiringPi Pin |
| ------ | ----------------------: |
| Potentiometer | MCP3208 CH1 |
| CDS Sensor | MCP3208 CH0 |
| MCP3208 CS | WiringPi 11 |
| Passive Buzzer | WiringPi 21 |
| Red LED | WiringPi 29 |

### Thresholds

| Condition | Range |
| --------- | ----- |
| Red LED ON | Key ±100 |
| Safe Open | Key ±10 |

The CDS sensor threshold is configured in the source code
and may need to be adjusted according to the actual sensor
value measured when the CDS sensor is covered.

### Build

```bash
gcc -Wall -o gold_mine main.c -lwiringPi
```

### Run

```bash
sudo ./gold_mine
```

## Learning Points

Through these examples, the following embedded system
concepts were practiced:

- SPI communication
- Analog sensor input using an ADC
- MCP3208 ADC channel selection
- RFID card detection and authentication
- RFID block read/write operations
- Storing application data on an RFID card
- Sensor threshold-based control
- LED and buzzer control
- Random value generation
- Raspberry Pi camera control
- Integration of multiple hardware devices

## Directory Structure

```text
05_Sensors_RFID/
├── README.md
│
├── rfid/
│   ├── main.c
│   ├── MFRC522.c
│   ├── MFRC522.h
│   └── README.md
│
├── rfid_traffic_card/
│   ├── main.c
│   └── README.md
│
└── gold_mine/
    ├── main.c
    └── README.md
```

## Source

Implemented based on the Sensors, RFID, and assignment
requirements from the Embedded Systems laboratory materials.