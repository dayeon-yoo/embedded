# Potentiometer

## Overview

A potentiometer sensor reading example using
Raspberry Pi SPI communication and the MCP3208 ADC.

The potentiometer value is read from the ADC
and continuously printed to the terminal.

## Hardware

- Raspberry Pi 3 Model B+
- Potentiometer
- MCP3208 ADC

## SPI Configuration

| Setting | Value |
| ------- | ----- |
| SPI Speed | 1 MHz |
| SPI CE Channel | 1 |
| ADC Channel | 1 |
| MCP3208 CS Pin | 11 |

## Implementation

- Initialize WiringPi
- Initialize SPI communication
- Configure the MCP3208 chip select pin
- Read the analog value from ADC channel 1
- Extract the 10-bit ADC value
- Print the potentiometer value every 500 ms

## Build

gcc main.c -o potentiometer -lwiringPi

## Run

sudo ./potentiometer

## Example

Potentiometer Value = 0
Potentiometer Value = 256
Potentiometer Value = 512
Potentiometer Value = 768
Potentiometer Value = 1023

## Source

Reconstructed from the Potentiometer
laboratory materials in the Embedded Systems course.