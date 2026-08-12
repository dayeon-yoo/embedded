# 06 Bluetooth & Android

## Overview

This section contains Bluetooth communication examples
between Raspberry Pi and an Android device.

The examples use Bluetooth RFCOMM / SPP communication
and the WiringPi serial interface.

The Raspberry Pi receives commands from an Android
application and sends sensor data or controls
hardware devices according to the received commands.

## Contents

| Directory | Description |
| --------- | ----------- |
| `bluetooth_basic` | Basic Bluetooth transmission and reception |
| `sensor_app` | Sensor data transmission through Bluetooth |
| `p5Activity` | Bluetooth-based DC motor and servo motor control |
| `lcd_control` | Bluetooth-based Text LCD control |
| `buzzer_control` | Bluetooth-based passive buzzer control |

## Communication

The Raspberry Pi communicates with the Android device
through a Bluetooth RFCOMM connection.

| Item | Setting |
| ---- | ------- |
| Device | `/dev/rfcomm0` |
| Baud Rate | `115200` |
| Communication | RFCOMM / SPP |

Bluetooth messages use `@` as the start of a command
and `#` as the end of a command.

Example:

```text
@TMP,32#
```

## 1. Bluetooth Basic Communication

The `bluetooth_basic` example demonstrates basic
Bluetooth serial communication.

### TX

`TX.c` periodically sends a test message:

```text
@TMP,32#
```

The message is transmitted once every second.

### RX

`RX.c` receives Bluetooth messages and detects
the end of each message using `#`.

Messages beginning with `@` are displayed.

### Build

```bash
gcc -o TX TX.c -lwiringPi
gcc -o RX RX.c -lwiringPi
```

### Run

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./TX
```

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./RX
```

## 2. Sensor Application

The `sensor_app` example reads three sensor values
and transmits them to an Android device.

### Sensors

| Sensor | Interface | Channel / Pin |
| ------ | --------- | ------------- |
| Ultrasonic Sensor | GPIO | TRIG 4 / ECHO 5 |
| Light Sensor | MCP3208 | CH0 |
| Potentiometer | MCP3208 | CH1 |

### Data Format

Sensor data is transmitted every second:

```text
@SENSOR,ULTRA,LIGHT,POTEN#
```

For example:

```text
@SENSOR,25,723,418#
```

### Implementation

- Read ultrasonic distance
- Read light sensor value
- Read potentiometer value
- Send the three sensor values through Bluetooth
- Repeat the operation every second

### Build

```bash
gcc -o sensor_app main.c -lwiringPi
```

### Run

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./sensor_app
```

## 3. P5 Activity - Motor Control

The `p5Activity` example controls a DC motor
and a servo motor through Bluetooth commands.

### Hardware

- DC Motor
- Servo Motor

### GPIO Mapping

| Device | WiringPi Pin |
| ------ | ------------ |
| DC Motor A | 6 |
| DC Motor B | 31 |
| Servo Motor | 26 |

### DC Motor Commands

| Command | Function |
| ------- | -------- |
| `@SPD,#` | Check current speed |
| `@MCW,#` | Clockwise rotation |
| `@MCC,#` | Counter-clockwise rotation |
| `@ACC,#` | Increase speed by 10 |
| `@DEC,#` | Decrease speed by 10 |
| `@STP,#` | Stop motor |

The DC motor speed is controlled from 0 to 100.

The initial speed is 50.

### Servo Commands

| Command | Function |
| ------- | -------- |
| `@SVL,#` | Move servo left |
| `@SVR,#` | Move servo right |

The servo uses a soft PWM range of 0 to 199.

### Speed Response

When the Android application requests the current
speed using:

```text
@SPD,#
```

the Raspberry Pi sends a response such as:

```text
@SPD,50#
```

## 4. LCD Control

The `lcd_control` example allows an Android device
to control a 16x2 Text LCD through Bluetooth.

### Functions

| Command | Function |
| ------- | -------- |
| `@LCD,text#` | Display text |
| `@LON,#` | Turn backlight ON |
| `@LOFF,#` | Turn backlight OFF |
| `@LCLR,#` | Clear LCD |

### Hardware

- 16x2 Text LCD
- PCF8574 I2C module

### LCD Configuration

| Setting | Value |
| ------- | ----- |
| PCF8574 Address | `0x27` |
| LCD Mode | 4-bit |
| LCD Size | 16x2 |

Text input is limited to 16 characters.

### Example

```text
@LCD,Hello#
```

The Raspberry Pi displays:

```text
Hello
```

on the LCD.

## 5. Buzzer Control

The `buzzer_control` example controls a passive
buzzer through Bluetooth commands.

The application supports individual notes and
predefined songs.

### Functions

| Command | Function |
| ------- | -------- |
| `@DO,#` | Do |
| `@RE,#` | Re |
| `@MI,#` | Mi |
| `@FA,#` | Fa |
| `@SOL,#` | Sol |
| `@LA,#` | La |
| `@SI,#` | Si |
| `@RABBIT,#` | Rabbit Song |
| `@STAR,#` | Twinkle Twinkle Little Star |
| `@STOP,#` | Stop music |

### Hardware

| Device | WiringPi Pin |
| ------ | ------------ |
| Passive Buzzer | 21 |

### Note Frequencies

| Note | Frequency |
| ---- | --------: |
| Do | 262 Hz |
| Re | 294 Hz |
| Mi | 330 Hz |
| Fa | 349 Hz |
| Sol | 392 Hz |
| La | 440 Hz |
| Si | 494 Hz |

### Threaded Playback

Song playback is handled using a dedicated
`PI_THREAD`.

This allows the main program to continue receiving
Bluetooth commands while a song is being played.

## Raspberry Pi Bluetooth Setup

The Bluetooth serial service uses RFCOMM.

An SPP service can be registered with:

```bash
sudo sdptool add --channel=4 SP
```

The Bluetooth RFCOMM device is then used as:

```text
/dev/rfcomm0
```

Programs can be executed through:

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./program
```

## Message Format

Bluetooth commands generally follow the format:

```text
@COMMAND,DATA#
```

where:

| Symbol | Meaning |
| ------ | ------- |
| `@` | Start of message |
| `COMMAND` | Command or data identifier |
| `DATA` | Command parameter or sensor data |
| `#` | End of message |

Examples:

```text
@TMP,32#
@SENSOR,25,723,418#
@LCD,Hello#
@SPD,50#
```

## Implementation Concepts

Through these examples, the following embedded
system concepts were practiced:

- Bluetooth RFCOMM communication
- Serial communication using `wiringSerial`
- Command parsing
- Message framing using start/end characters
- Periodic sensor data transmission
- Bluetooth-based hardware control
- DC motor soft PWM control
- Servo motor soft PWM control
- Text LCD control through PCF8574
- Passive buzzer control
- Thread-based music playback
- Raspberry Pi and Android communication

## Directory Structure

```text
06_Bluetooth_Android/
├── README.md
│
├── bluetooth_basic/
│   ├── TX.c
│   ├── RX.c
│   └── README.md
│
├── sensor_app/
│   ├── main.c
│   └── README.md
│
├── p5Activity/
│   ├── main.c
│   └── README.md
│
├── lcd_control/
│   ├── main.c
│   └── README.md
│
└── buzzer_control/
    ├── main.c
    └── README.md
```

## Source

Implemented and reconstructed based on the Bluetooth
communication and Android application requirements
from the Embedded Systems laboratory materials.