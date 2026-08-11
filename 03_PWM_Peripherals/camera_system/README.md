# Ultrasonic Camera System

## Overview

An integrated embedded system that detects an approaching
object using an ultrasonic sensor and captures an image
when the object remains within a specified distance.

The system combines ultrasonic distance measurement,
buzzer warning, LED indication, and Raspberry Pi camera
control.

## Hardware

- Raspberry Pi 3 Model B+
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- Red LED
- Raspberry Pi Camera

## GPIO Mapping

| Component | WiringPi Pin |
|---|---:|
| Ultrasonic TRIG | 4 |
| Ultrasonic ECHO | 5 |
| Active Buzzer | 30 |
| Red LED | 29 |

## Implementation

- Wait for the user to press Enter before starting
- Measure distance using the HC-SR04 ultrasonic sensor
- Detect an object within 20 cm
- Activate the buzzer with a short periodic warning for 3 seconds
- Measure the distance again after the warning
- Capture an image if the object is still within 20 cm
- Turn on the red LED during the camera operation
- Print `Take a Picture!` after requesting the image capture
- Save the captured image as `captured.jpeg`

## Control Flow

```text
Enter
  |
  v
Measure Distance
  |
  v
Within 20 cm?
  |
  +---- No ----> Continue Monitoring
  |
 Yes
  |
  v
Buzzer Warning
  |
  v
Measure Distance Again
  |
  +---- No ----> Continue Monitoring
  |
 Yes
  |
  v
Red LED ON
  |
  v
Camera Capture
  |
  v
Save Image
  |
  v
Red LED OFF
```

## Camera Capture

The program uses the Raspberry Pi camera command
`libcamera-jpeg` through the C `system()` function.

```bash
sudo libcamera-jpeg --width 800 --height 800 -t 1 -o captured.jpeg
```

## Build

```bash
gcc main.c -o camera_system -lwiringPi
```

## Run

```bash
sudo ./camera_system
```

Press Enter to start the system.

## Output

When an object remains within 20 cm after the buzzer warning,
the system captures an image and prints:

```text
Take a Picture!
```

## Source

Reconstructed from the camera example and assignment
requirements in the Embedded Systems laboratory materials.