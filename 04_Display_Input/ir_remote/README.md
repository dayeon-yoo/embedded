# IR Remote Control

## Overview

An infrared remote control example using
Raspberry Pi GPIO.

The program receives infrared remote control signals
and controls an LED according to the received button code.

## Controls

| Remote Button | Action |
| ------------- | ------ |
| Button 0 | LED ON |
| Button 1 | LED OFF |

## Hardware

- Raspberry Pi 3 Model B+
- IR Receiver
- LED

## GPIO Mapping

| Device | WiringPi Pin |
| ------ | -----------: |
| IR Receiver | 18 |
| LED | 27 |

## Remote Codes

| Button | IR Code |
| ------ | -------- |
| Button 0 | `0x102a4f46` |
| Button 1 | `0x8534077e` |

## Implementation

- Initialize the IR receiver and Raspberry Pi GPIO
- Configure the IR pin as an input
- Set a 5 ms watchdog interval for the IR signal
- Monitor IR signal level changes using an alert callback
- Receive IR codes using `readIR()`
- Compare the received code with predefined button codes
- Turn the LED on when Button 0 is pressed
- Turn the LED off when Button 1 is pressed

## Build

```bash
gcc -o ir_remote main.c irpigpio.c -lpthread -lwiringPi -lpigpio
```

## Run

```bash
sudo ./ir_remote
```

## Example

```text
Button 0 pressed, LED On
Button 1 pressed, LED Off
```

## Source

Reconstructed from the IR remote control example
in the Embedded Systems laboratory materials.

The IR signal processing functions are provided through
the `irpigpio` driver used by the laboratory example.