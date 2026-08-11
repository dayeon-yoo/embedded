# GPIO Control

Raspberry Pi 3 Model B+의 GPIO를 활용하여 LED, Push Switch,
Active Buzzer 등의 기본적인 하드웨어 입출력을 제어한 실습입니다.

## Overview

이 디렉토리에서는 Raspberry Pi의 GPIO를 이용한 디지털 입력과
출력을 다루고, 입력 장치의 상태에 따라 출력 장치를 제어하는
기본적인 임베디드 제어를 구현했습니다.

### Key Concepts

- GPIO Output Control
- GPIO Input Handling
- Digital I/O
- LED State Control
- Switch Input
- Buzzer Control
- Basic State Management
- Switch Debouncing

## Implementations

| Implementation | Description | GPIO |
|---|---|---|
| [RGB LED Control](./rgb_led/) | RGB LED의 각 채널을 순차적으로 제어 | Output |
| [Push Switch Input](./push_switch/) | 두 개의 Push Switch 입력 감지 | Input |
| [Active Buzzer Control](./buzzer_control/) | Active Buzzer ON/OFF 제어 | Output |
| [LED State Control](./led_switch_control/) | Switch 입력에 따라 RGB LED 상태 변경 | Input + Output |
| [Buzzer Switch Control](./buzzer_switch_control/) | Switch 입력으로 Buzzer ON/OFF 토글 | Input + Output |

## Hardware

- Raspberry Pi 3 Model B+
- RGB LED
- Push Switch
- Active Buzzer

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Push Switch (Left) | 0 |
| Push Switch (Right) | 7 |
| Blue LED | 27 |
| Green LED | 28 |
| Red LED | 29 |
| Active Buzzer | 30 |

## Control Flow

### Basic GPIO Output

```text
Raspberry Pi
     |
     +-- GPIO Output --> RGB LED
     |
     +-- GPIO Output --> Buzzer
```

### GPIO Input and Output Control

```text
Push Switch
     |
     v
GPIO Input
     |
     v
Control Logic
     |
     +----> RGB LED
     |
     +----> Buzzer
```

## Environment

- Raspberry Pi 3 Model B+
- Raspberry Pi OS
- C
- WiringPi

## Source Status

The examples in this directory were recovered or reconstructed
from Embedded Systems laboratory materials.

- `rgb_led` - Recovered from lecture material
- `push_switch` - Recovered from lecture material
- `buzzer_control` - Recovered from lecture material
- `led_switch_control` - Reconstructed from assignment requirements
- `buzzer_switch_control` - Reconstructed from assignment requirements