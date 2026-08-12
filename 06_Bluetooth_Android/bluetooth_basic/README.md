# Bluetooth Basic Communication

## Overview

A basic Bluetooth serial communication example
using Raspberry Pi and the wiringSerial library.

The program demonstrates Bluetooth data transmission
and reception through the RFCOMM device.

## Communication

| Program | Function |
| ------- | -------- |
| `TX.c` | Send Bluetooth data |
| `RX.c` | Receive Bluetooth data |

## Communication Format

The transmitted data uses the following format:

```text
@TMP,32#
```

| Symbol | Meaning |
| ------ | ------- |
| `@` | Start of data |
| `TMP` | Data type |
| `32` | Data value |
| `#` | End of data |

## Bluetooth Configuration

- Device: `/dev/rfcomm0`
- Baud rate: `115200`
- Communication type: RFCOMM / SPP

## TX Implementation

- Open the Bluetooth serial device
- Initialize WiringPi
- Send virtual sensor data every 1 second
- Transmit `@TMP,32#`

## RX Implementation

- Open the Bluetooth serial device
- Initialize WiringPi
- Check received data every 250 ms
- Store received characters in a buffer
- Detect `#` as the end of the message
- Print data beginning with `@`

## Build

### TX

```bash
gcc -o TX TX.c -lwiringPi
```

### RX

```bash
gcc -o RX RX.c -lwiringPi
```

## Run

### TX

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./TX
```

### RX

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./RX
```

## Example

### TX

```text
Bluetooth init
Send Sensor Data
Send Sensor Data
Send Sensor Data
```

### RX

```text
Bluetooth init
RX Data = @TMP,32#
RX Data = @TMP,32#
RX Data = @TMP,32#
```

## Source

Reconstructed from the Bluetooth communication
examples in the Embedded Systems laboratory materials.