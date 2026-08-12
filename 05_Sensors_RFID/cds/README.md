# CDS Light Sensor

## Overview

A CDS light sensor reading example using
Raspberry Pi SPI communication and the MCP3208 ADC.

The CDS sensor value is read from the ADC
and continuously printed to the terminal.

## Hardware

- Raspberry Pi 3 Model B+
- CDS Light Sensor
- MCP3208 ADC

## SPI Configuration

| Setting | Value |
| ------- | ----- |
| SPI Speed | 1 MHz |
| SPI CE Channel | 1 |
| ADC Channel | 0 |
| MCP3208 CS Pin | 11 |

## Implementation

- Initialize WiringPi
- Initialize SPI communication
- Configure the MCP3208 chip select pin
- Read the analog value from ADC channel 0
- Extract the 10-bit ADC value
- Print the light sensor value every 500 ms

## Build

```bash
gcc main.c -o cds -lwiringPi
```

## Run

```bash
sudo ./cds
```

## Example

```bash
Light Value = 0Lux
Light Value = 256Lux
Light Value = 512Lux
Light Value = 768Lux
Light Value = 1023Lux
```

## Source

Reconstructed from the CDS light sensor
laboratory materials in the Embedded Systems course.