# PWM & Peripheral Control

## Overview

Raspberry Pi의 Soft PWM과 주변장치를 이용하여
LED, DC Motor, Servo Motor, Passive Buzzer, Camera를
제어한 실습입니다.

기본적인 PWM 출력 제어부터 키보드 입력과 여러
하드웨어를 결합한 통합 시스템까지 단계적으로 구현했습니다.

## Hardware

- Raspberry Pi 3 Model B+
- LED
- DC Motor
- BA6208 Motor Driver
- Servo Motor
- Passive Buzzer
- Raspberry Pi Camera
- HC-SR04 Ultrasonic Sensor

## Implementations

| Implementation | Description | Control Method |
|---|---|---|
| [PWM LED Control](./pwm_led/) | PWM duty cycle을 이용한 LED 밝기 제어 | Soft PWM |
| [PWM DC Motor Control](./pwm_dc_motor/) | PWM duty cycle을 이용한 DC Motor 속도 및 방향 제어 | Soft PWM |
| [Servo Motor Control](./servo_motor/) | PWM을 이용한 Servo Motor 위치 제어 | Soft PWM |
| [Passive Buzzer Control](./passive_buzzer/) | 주파수를 변경하여 8개 음계 출력 | Soft Tone |
| [Servo Motor Keyboard Control](./servo_motor_keyboard/) | 키보드 입력에 따른 Servo Motor 위치 및 LED 밝기 제어 | Keyboard + Soft PWM |
| [Piano Buzzer](./piano_buzzer/) | 키보드 입력에 따른 8개 음계 연주 | Keyboard + Soft Tone |
| [Ultrasonic Camera System](./camera_system/) | 거리 감지 후 부저 경고 및 카메라 촬영 | Sensor + Actuator + Camera |

## GPIO Mapping

### PWM LED

| Component | WiringPi Pin |
|---|---:|
| Red LED | 27 |

### DC Motor

| Component | WiringPi Pin |
|---|---:|
| Motor Input A | 6 |
| Motor Input B | 31 |

### Servo Motor

| Component | WiringPi Pin |
|---|---:|
| Servo Motor | 26 |

### Passive Buzzer

| Component | WiringPi Pin |
|---|---:|
| Passive Buzzer | 1 |

### Servo Motor Keyboard Control

| Component | WiringPi Pin |
|---|---:|
| Servo Motor | 26 |
| LED | 27 |

### Piano Buzzer

| Component | WiringPi Pin |
|---|---:|
| Passive Buzzer | 21 |

### Ultrasonic Camera System

| Component | WiringPi Pin |
|---|---:|
| Ultrasonic TRIG | 4 |
| Ultrasonic ECHO | 5 |
| Active Buzzer | 30 |
| Red LED | 29 |

## Key Concepts

### Soft PWM

Soft PWM을 이용하여 GPIO 출력의 duty cycle을
조절하고 LED 밝기와 DC Motor 속도를 제어했습니다.

### Servo Motor Control

PWM 신호의 pulse width를 변경하여 Servo Motor의
위치를 제어했습니다.

### Soft Tone

`softTone()`을 이용하여 Passive Buzzer에 서로 다른
주파수를 출력하고 음계를 생성했습니다.

### Keyboard Input

Linux `termios` 기반의 `getch()`를 이용하여
Enter 입력 없이 키보드 입력을 처리했습니다.

### Hardware Integration

Ultrasonic Sensor, Buzzer, LED, Camera를 결합하여
거리 조건에 따라 장치가 순차적으로 동작하는
통합 시스템을 구현했습니다.

## Control Flow

```text
PWM Output
    |
    +----> LED Brightness
    |
    +----> DC Motor Speed
    |
    +----> Servo Motor Position

Keyboard Input
    |
    +----> Servo Motor
    |
    +----> Buzzer / Piano

Ultrasonic Sensor
    |
    v
Distance Measurement
    |
    v
Condition Check
    |
    +----> Buzzer Warning
    |
    +----> Camera Capture
```

## Source Status

The examples in this directory were recovered or reconstructed
from the Embedded Systems laboratory materials.

- `pwm_led` — Recovered from lecture material
- `pwm_dc_motor` — Recovered from lecture material
- `servo_motor` — Recovered from lecture material
- `passive_buzzer` — Recovered from lecture material
- `servo_motor_keyboard` — Reconstructed from assignment requirements
- `piano_buzzer` — Reconstructed from assignment requirements
- `camera_system` — Reconstructed from assignment requirements and camera example