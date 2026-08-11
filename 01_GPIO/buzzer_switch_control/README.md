# Buzzer Control with Push Switch

## Overview

A GPIO input/output control example that uses a push switch
to toggle an active buzzer.

The buzzer starts in the OFF state and changes its state
each time the switch is pressed.

## Behavior

```text
Initial
  ↓
Buzzer OFF
  ↓ Switch Press
Buzzer ON
  ↓ Switch Press
Buzzer OFF
  ↓
Repeat
```

## Hardware

- Raspberry Pi 3 Model B+
- Push Switch
- Active Buzzer

## GPIO Mapping

| Component | WiringPi Pin |
|-----------|--------------|
| Push Switch | 0 |
| Active Buzzer | 30 |

## Implementation

- Configure the push switch as a GPIO input
- Configure the active buzzer as a GPIO output
- Detect a HIGH-to-LOW transition as a button press
- Toggle the buzzer state on each press
- Apply a short delay for switch debouncing
- Keep the buzzer state independent from the debounce delay

## Build

```bash
gcc main.c -o buzzer_switch_control -lwiringPi
```

## Run

```bash
sudo ./buzzer_switch_control
```

Press `Ctrl + C` to stop the program.

## Source

Reconstructed from the GPIO assignment requirements
in the Embedded Systems laboratory materials.