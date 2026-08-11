# Push Switch Input

## Overview

A basic GPIO input control example using push switches
and the WiringPi library on Raspberry Pi.

The program monitors two push switches and prints a message
when either switch is pressed.

## Hardware

- Raspberry Pi 3 Model B+
- Push Switch × 2

## GPIO Mapping

| Component | WiringPi Pin |
|-----------|--------------|
| Left Switch | 0 |
| Right Switch | 7 |

## Implementation

- Initialize Raspberry Pi GPIO using WiringPi
- Configure both switch pins as `INPUT`
- Read switch states using `digitalRead()`
- Detect a pressed switch when the input is `LOW`
- Print the corresponding switch state
- Apply a 300 ms delay after detecting a press

## Example Output

```text
Left Switch Pushed!
Right Switch Pushed!
```

## Build

```bash
gcc main.c -o push_switch -lwiringPi
```

## Run

```bash
sudo ./push_switch
```

Press `Ctrl + C` to stop the program.

## Source

Recovered from Embedded Systems laboratory lecture material.