# Bluetooth Sensor Application

## Overview

A Raspberry Pi sensor application that reads
ultrasonic, light, and potentiometer values
and transmits the sensor data through Bluetooth.

The sensor values are measured periodically
and sent to an Android device through
the Bluetooth RFCOMM connection.

## Sensors

| Sensor | Interface | Channel / Pin |
| ------ | --------- | ------------- |
| Ultrasonic Sensor | GPIO | TRIG 4 / ECHO 5 |
| Light Sensor | SPI ADC | MCP3208 CH0 |
| Potentiometer | SPI ADC | MCP3208 CH1 |

## Bluetooth

| Item | Setting |
| ---- | ------- |
| Device | `/dev/rfcomm0` |
| Baud Rate | `115200` |
| Communication | RFCOMM / SPP |

## Data Format

Sensor data is transmitted using the following format:

```text
@SENSOR,ULTRA,LIGHT,POTEN#
```

| Field | Description |
| ----- | ----------- |
| `@` | Start of message |
| `SENSOR` | Sensor data identifier |
| `ULTRA` | Ultrasonic distance in cm |
| `LIGHT` | Light sensor ADC value |
| `POTEN` | Potentiometer ADC value |
| `#` | End of message |

## Implementation

- Initialize WiringPi
- Initialize SPI communication with MCP3208
- Initialize the ultrasonic sensor
- Open the Bluetooth RFCOMM device
- Read the light sensor through MCP3208 channel 0
- Read the potentiometer through MCP3208 channel 1
- Measure the ultrasonic sensor distance
- Send sensor data through Bluetooth every 1 second

## Hardware

- Raspberry Pi 3 Model B+
- MCP3208 ADC
- Ultrasonic Sensor
- Light Sensor
- Potentiometer
- Bluetooth SPP connection

## GPIO Mapping

| Device | WiringPi Pin |
| ------ | ------------ |
| MCP3208 CS | 11 |
| Ultrasonic TRIG | 4 |
| Ultrasonic ECHO | 5 |

## Build

```bash
gcc -o sensor_app main.c -lwiringPi
```

## Run

```bash
sudo rfcomm watch /dev/rfcomm0 4 ./sensor_app
```

## Example

```text
Bluetooth init OK (/dev/rfcomm0)

Sensor Data - ULTRA=25, LIGHT=723, POTEN=418
TX = @SENSOR,25,723,418#

Sensor Data - ULTRA=24, LIGHT=719, POTEN=421
TX = @SENSOR,24,719,421#
```

## Source

Reconstructed from the Bluetooth sensor communication
requirements in the Embedded Systems laboratory materials.