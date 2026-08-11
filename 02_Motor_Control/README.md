# Motor Control

Raspberry Pi 3 Model B+와 GPIO를 이용하여 DC Motor와
Stepper Motor를 제어한 실습입니다.

기본적인 모터 방향 제어부터 키보드 입력을 이용한
모터 제어까지 단계적으로 구현했습니다.

## Overview

이 디렉토리에서는 Raspberry Pi GPIO를 이용한
actuator control을 다룹니다.

### Key Concepts

- GPIO Output Control
- DC Motor Direction Control
- Stepper Motor Control
- Four-Phase Step Sequence
- Keyboard Input
- Motor State Control
- Direction Control

## Implementations

| Implementation | Description | Control Method |
|---|---|---|
| [DC Motor Control](./dc_motor/) | DC Motor의 정방향, 역방향 및 정지 제어 | GPIO |
| [Stepper Motor Control](./step_motor/) | 4상 제어를 이용한 Stepper Motor 회전 | GPIO |
| [DC Motor Keyboard Control](./dc_motor_keyboard/) | 키보드 입력을 이용한 DC Motor 방향 및 정지 제어 | Keyboard + GPIO |
| [Stepper Motor Keyboard Control](./step_motor_keyboard/) | 입력한 각도에 따라 Stepper Motor의 방향과 회전을 제어 | Keyboard + GPIO |

## Hardware

- Raspberry Pi 3 Model B+
- DC Motor
- BA6208 Motor Driver
- Stepper Motor
- Stepper Motor Driver
- RGB LED

## GPIO Mapping

### DC Motor

| Component | WiringPi Pin |
|---|---:|
| Motor Input A | 6 |
| Motor Input B | 31 |

### Stepper Motor

| Motor Phase | WiringPi Pin |
|---|---:|
| Blue | 22 |
| Pink | 23 |
| Yellow | 24 |
| Orange | 25 |

### RGB LED

| LED | WiringPi Pin |
|---|---:|
| Blue | 27 |
| Green | 28 |
| Red | 29 |

## Control Flow

### DC Motor

```text
Keyboard Input
      |
      v
Control Command
      |
      +---- "+" ----> CW
      |
      +---- "-" ----> CCW
      |
      +---- "0" ----> STOP
      |
      v
GPIO Output
      |
      v
DC Motor
```

### Stepper Motor

```text
Rotation Input
      |
      v
Direction Selection
      |
      v
Step Sequence
      |
      v
GPIO Output
      |
      v
Stepper Motor
```

## Stepper Motor Control

The stepper motor is controlled using a four-phase
GPIO sequence.

### Clockwise

```text
HIGH HIGH LOW  LOW
LOW  HIGH HIGH LOW
LOW  LOW  HIGH HIGH
HIGH LOW  LOW  HIGH
```

### Counter-clockwise

```text
HIGH LOW  LOW  HIGH
LOW  LOW  HIGH HIGH
LOW  HIGH HIGH LOW
HIGH HIGH LOW  LOW
```

## Keyboard Input

The keyboard-controlled DC motor implementation uses the
Linux `termios` interface to receive keyboard input without
requiring the Enter key.

The DC motor control uses the following commands:

| Key | Action |
|---|---|
| `+` | Clockwise |
| `-` | Counter-clockwise |
| `0` | Stop |
| `q` | Quit |

## Source Status

The examples in this directory were recovered or reconstructed
from Embedded Systems laboratory materials.

- `dc_motor` — Recovered from lecture material
- `step_motor` — Recovered from lecture material
- `dc_motor_keyboard` — Reconstructed from assignment requirements and keyboard input material
- `step_motor_keyboard` — Reconstructed from assignment requirements