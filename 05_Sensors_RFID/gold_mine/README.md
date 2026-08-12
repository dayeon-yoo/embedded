# Gold Mine Safe

## Overview

A safe system using a potentiometer, CDS sensor,
passive buzzer, red LED, and Raspberry Pi camera.

The system generates a random key value and uses the
potentiometer to find the key.

## Operation

1. The system waits for the Enter key.
2. When Enter is pressed, a random key value between 10 and 1000 is generated.
3. The potentiometer value is periodically displayed.
4. When the potentiometer value enters the key value ±100 range, the red LED turns on.
5. When the potentiometer value enters the key value ±10 range, the safe opens.
6. An ascending scale is played through the passive buzzer.
7. The CDS sensor checks the light level after the safe is opened.
8. If the light level is low, the camera takes a picture.
9. The program terminates after all operations are completed.

## Hardware

- Raspberry Pi 3 Model B+
- Potentiometer
- MCP3208 ADC
- CDS Sensor
- Passive Buzzer
- RGB LED
- Raspberry Pi Camera

## GPIO / SPI Mapping

### MCP3208

| Setting | Value |
| ------- | ----- |
| SPI Speed | 1 MHz |
| SPI CE Channel | 1 |
| MCP3208 CS Pin | 11 |
| Potentiometer ADC Channel | 1 |
| CDS ADC Channel | 0 |

### Output Devices

| Device | WiringPi Pin |
| ------ | ------------: |
| Passive Buzzer | 21 |
| Red LED | 29 |

## Thresholds

| Condition | Range |
| --------- | ----- |
| Red LED ON | Key ±100 |
| Safe Open | Key ±10 |
| CDS Dark Detection | ADC value < 300 |

The assignment specifies that the CDS value should be
checked against the value reached when the sensor is covered
by a person's hand.

The exact numerical threshold depends on the hardware
environment, so `CDS_DARK_THRESHOLD` can be adjusted after
measuring the actual CDS values.

## Implementation

- Initialize WiringPi
- Initialize SPI communication
- Configure the MCP3208 ADC
- Wait for the Enter key
- Generate a random key value between 10 and 1000
- Read the potentiometer value from ADC channel 1
- Turn on the red LED when the potentiometer is within ±100 of the key
- Open the safe when the potentiometer is within ±10 of the key
- Play an ascending scale using the passive buzzer
- Read the CDS value from ADC channel 0
- Detect a dark environment
- Capture an image using the Raspberry Pi camera
- Terminate the program after completing the operation

## Build

```bash
gcc -Wall -o gold_mine main.c -lwiringPi
```

## Run

```bash
sudo ./gold_mine
```

## Example

```text
Press Enter to start the safe.

Key Value = 527
Turn the potentiometer to find the key.

Potentiometer Value = 312
Potentiometer Value = 401
Potentiometer Value = 468
Potentiometer Value = 521

Safe Open!
CDS Value = 180
Dark environment detected.
Take a Picture!
Picture saved.
```

## Camera

The program uses the Raspberry Pi camera command:

```bash
libcamera-jpeg -o gold_mine.jpg -t 1000
```

The captured image is saved as `gold_mine.jpg`.

## Source

Implemented based on the Gold Mine Safe assignment
requirements from the Embedded Systems laboratory materials.