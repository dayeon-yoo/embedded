# Bluetooth Android IoT Practice

Android 스마트폰/태블릿과 Raspberry Pi를 Bluetooth로 연결하여 Raspberry Pi의 IoT 장치를 Android 애플리케이션에서 제어하고 센서 데이터를 확인하는 실습 프로젝트입니다.

Android에서 Bluetooth 장치를 검색하고 연결한 뒤, Bluetooth Serial 통신을 통해 Raspberry Pi에 제어 명령을 전달하도록 구현했습니다.

---

## Project Overview

본 프로젝트에서는 Android 애플리케이션과 Raspberry Pi 사이의 **Bluetooth SPP(Serial Port Profile) 기반 통신**을 구현했습니다.

Android 앱에서 페어링된 Bluetooth 장치를 확인하고 원하는 장치를 선택하여 연결할 수 있으며, 연결 이후 버튼 입력을 통해 Raspberry Pi에 명령을 전송합니다.

Raspberry Pi는 전달받은 명령을 바탕으로 모터 및 서보 모터 등을 제어하고, 센서 측정값을 Android로 다시 전달합니다.

### 전체 통신 구조

```text
┌──────────────────────┐
│    Android Device    │
│                      │
│  Bluetooth App       │
│  - Device Selection  │
│  - Bluetooth Connect │
│  - Device Control    │
│  - Sensor Monitoring │
└──────────┬───────────┘
           │
           │ Bluetooth SPP
           │ Serial Communication
           ▼
┌──────────────────────┐
│     Raspberry Pi     │
│                      │
│  Bluetooth Server    │
│  - Command Receive   │
│  - Motor Control     │
│  - Servo Control     │
│  - Sensor Reading    │
└──────────────────────┘
```

---

## 주요 기능

### 1. Bluetooth 장치 연결

- Android Bluetooth Adapter 확인
- Bluetooth 활성화 요청
- 페어링된 Bluetooth 장치 목록 조회
- 장치 이름 및 MAC Address 표시
- 선택한 Raspberry Pi Bluetooth 장치 연결
- Bluetooth 연결 상태 확인

### 2. Raspberry Pi 제어

Android의 버튼 입력을 Bluetooth 메시지로 변환하여 Raspberry Pi로 전달합니다.

```text
Android Button
      ↓
Command 생성
      ↓
Bluetooth SPP
      ↓
Raspberry Pi
      ↓
GPIO / Motor / Servo 제어
```

### 3. DC Motor 제어

Android 앱에서 방향 제어 명령을 전송하여 Raspberry Pi에 연결된 DC 모터를 제어합니다.

- 정방향
- 역방향
- 좌/우 방향 제어
- 모터 정지

### 4. Servo Motor 제어

Android에서 서보 모터 제어 명령을 전송하여 Raspberry Pi의 서보 모터 동작을 제어합니다.

### 5. 센서 데이터 확인

Raspberry Pi에서 측정한 센서 데이터를 Bluetooth를 통해 Android로 전달하고 앱에서 결과를 확인할 수 있도록 구현했습니다.

속도 확인 명령은 다음과 같은 형태로 전송됩니다.

```text
@SPD,#
```

Raspberry Pi에서 측정한 결과는 Bluetooth를 통해 Android로 전달됩니다.

예시:

```text
@SPD,50#
```

---

## Android Application

### 주요 Activity

```text
MainActivity
    │
    └── IoTPractice
          │
          ├── p5Activity
          │
          └── DeviceListActivity
```

### `p5Activity`

Bluetooth 통신 및 IoT 장치 제어를 담당하는 메인 화면입니다.

주요 기능:
- Bluetooth 연결
- 센서 데이터 요청
- DC Motor 제어
- Servo Motor 제어
- Raspberry Pi로 명령 전송
- Bluetooth로 수신한 데이터 처리

### `DeviceListActivity`

Android에 페어링되어 있는 Bluetooth 장치를 표시하고 사용자가 연결할 장치를 선택할 수 있도록 구현했습니다.

장치 선택 시 Bluetooth 장치의 MAC Address를 전달하여 `p5Activity`에서 연결을 수행합니다.

---

## Bluetooth Communication

Bluetooth 통신은 별도의 `BTService` 클래스를 사용하여 관리합니다.

```text
BTService
 ├── connect()
 ├── stop()
 ├── write()
 ├── getState()
 └── Bluetooth data receive
```

### 데이터 송신

Android에서 생성한 문자열 명령을 byte 배열로 변환하여 Bluetooth를 통해 Raspberry Pi로 전달합니다.

```java
mBTService.write(message.getBytes());
```

### 데이터 수신

Raspberry Pi에서 전송한 데이터를 Android에서 수신하고 필요한 데이터를 추출하여 처리합니다.

---

## Development Environment

| 구분 | 사용 기술 |
|---|---|
| Mobile | Android |
| Language | Java |
| IDE | Android Studio |
| Communication | Bluetooth |
| Bluetooth Profile | SPP |
| Embedded Device | Raspberry Pi |
| Embedded Language | C |
| Raspberry Pi Library | wiringPi |
| Protocol | Bluetooth Serial Communication |

---

## Project Structure

```text
06_Bluetooth_Android/
│
├── android/
│   └── app/
│       └── src/
│           └── main/
│               ├── java/
│               │   └── com/
│               │       └── IoTPractice/
│               │           ├── Bluetooth/
│               │           │   └── BTService.java
│               │           │
│               │           ├── data/
│               │           │
│               │           ├── DeviceListActivity.java
│               │           ├── MainActivity.java
│               │           └── p5Activity.java
│               │
│               └── res/
│                   └── layout/
│                       ├── activity_main.xml
│                       └── activity_p5.xml
│
└── README.md
```

---

## 실행 방법

### 1. Raspberry Pi Bluetooth Server 실행

Raspberry Pi에서 Bluetooth SPP 서버를 실행합니다.

```bash
sudo sdptool add --channel=4 SP
sudo rfcomm watch /dev/rfcomm0 4 ./BTServer
```

Raspberry Pi의 Bluetooth가 Android 장치와 연결될 수 있도록 준비합니다.

### 2. Android Bluetooth 설정

Android 장치에서 Raspberry Pi를 Bluetooth 장치로 검색하고 페어링합니다.

### 3. Android 애플리케이션 실행

Android Studio에서 프로젝트를 실행합니다.

앱 실행 후 다음 순서로 Bluetooth 장치를 연결합니다.

```text
Bluetooth 활성화
      ↓
Bluetooth 버튼 선택
      ↓
페어링된 장치 목록 확인
      ↓
Raspberry Pi 선택
      ↓
Bluetooth 연결
```

### 4. IoT 장치 제어

Bluetooth 연결이 완료되면 Android 앱의 제어 버튼을 이용하여 Raspberry Pi에 명령을 전송합니다.

---

## Communication Example

### Android → Raspberry Pi

```text
Android
   │
   │ "@SPD,#"
   ▼
Bluetooth SPP
   │
   ▼
Raspberry Pi
   │
   └── 속도 측정
```

### Raspberry Pi → Android

```text
Raspberry Pi
   │
   │ "@SPD,50#"
   ▼
Bluetooth SPP
   │
   ▼
Android
   │
   └── 수신 데이터 처리
```

---

## Learning Objectives

- Android Bluetooth API 사용
- Bluetooth 장치 검색 및 연결
- 페어링된 Bluetooth 장치 관리
- Bluetooth SPP 기반 Serial 통신
- Android Activity 간 데이터 전달
- Android에서 Raspberry Pi로 제어 명령 전송
- Raspberry Pi에서 Bluetooth 데이터 수신
- 센서 데이터의 양방향 통신
- Bluetooth 기반 IoT 시스템 구성

---

## Project Result

Android 애플리케이션과 Raspberry Pi를 Bluetooth로 연결하여 **모바일 장치에서 임베디드 장치를 원격으로 제어하는 IoT 시스템**을 구현했습니다.

Android에서는 Bluetooth 장치를 선택하고 연결한 후 제어 명령을 전송하고, Raspberry Pi에서는 전달받은 명령에 따라 모터 및 서보 등의 장치를 제어하거나 센서 데이터를 측정하여 다시 Android로 전달하는 **양방향 Bluetooth 통신 구조**를 구성했습니다.