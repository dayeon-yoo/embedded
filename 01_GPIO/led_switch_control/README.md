# LED State Control with Push Switch

## Overview

A GPIO input/output control example that changes the RGB LED
state whenever the push switch is pressed.

The LED state changes in the following order:

```text
OFF
 ↓
GREEN
 ↓
RED
 ↓
BLUE
 ↓
OFF
```

## Hardware

- Raspberry Pi 3 Model B+
- RGB LED
- Push Switch

## GPIO Mapping

| Component | WiringPi Pin |
|-----------|--------------|
| Push Switch | 0 |
| Blue LED | 27 |
| Green LED | 28 |
| Red LED | 29 |

## Implementation

- Configure the push switch as a GPIO input
- Configure RGB LED channels as GPIO outputs
- Detect a switch press using a HIGH-to-LOW transition
- Change the LED state on each button press
- Apply a short delay for switch debouncing

## State Transition

| State | LED |
|------:|-----|
| 0 | OFF |
| 1 | GREEN |
| 2 | RED |
| 3 | BLUE |

## Build

```bash
gcc main.c -o led_switch_control -lwiringPi
```

## Run

```bash
sudo ./led_switch_control
```

Press `Ctrl + C` to stop the program.

## Source

Reconstructed from the GPIO assignment requirements
in the Embedded Systems laboratory materials.