# **Name - ESP8266 talk with robot ARM over I2C**

Short description... Micropython talks with Arduino Robot arm over I2C


# Things used in this project

### **Hardware components**

||Name|Qt||
| :---|:------------------------:|:---| ----:|
| Img | Raspberry Pi 3 Model B	 | ×1 | link |
| Img | Raspberry Pi 3 Model B	 | ×1 | link |
| Img | Raspberry Pi 3 Model B	 | ×1 | link |
| Img | Raspberry Pi 3 Model B	 | ×1 | link |
| Img | Raspberry Pi 3 Model B	 | ×1 | link |

### **Software apps and online services**
||Name|Qt||
| :---|:------------------------:|:---| ----:|
| Img | Raspberry Pi 3 Model B	 | ×1 | link |

### **Hand tools and fabrication machines**
||Name|Qt||
| :---|:------------------------:|:---| ----:|
| Img | Raspberry Pi 3 Model B	 | ×1 | link |



# Story



#### Description

Arm of the robot for self-assembly, equipped with 6 servos. There is also a special controller Braccio in the form of cap over the  [Arduino](https://botland.com.pl/en/283-arduino-core-modules)  and the necessary mounting elements. The device is powered with 5 volts, consumes a maximum current of 5 A. (includes power supply).

After assemblying the arm, to verify its working, just run the example program testBraccio90 directly from the  [environment of Arduino IDE](http://www.arduino.org/downloads), in section _Braccio_.

#### Key features

-   A kit for self-assembly, compatible with the Arduino plates
-   The driver in the form of cap on the Arduino: Braccio Shield
-   4 servos with moment up to 3.8 kg*cm
-   2 servos with moment up to 14.5 kg. cm
-   Included is power adapter 5 V / 5 VA
-   Available are sample programs directly from  [the Arduino IDE environment](http://www.arduino.org/downloads)
-   Available is detailed  [user guide](http://download.arduino.org/products/braccio/Braccio_Quick_Start_Guide.pdf)
-   Examples of use: object tracking using the webcam, rotation of the screen during a videoconference, tracking the sun with solar panel

![Braccio robot arm](/images/robot1.jpg =100x100)
![Braccio robot arm](/images/robot2.jpg)

[Read the user manual!](http://download.arduino.org/products/braccio/Braccio_Quick_Start_Guide.pdf)

#### Control - Arduino Shield

The kit includes a controller as cap for Arduino, so-called Braccio Arduino Shield. The device can control 6 motors / servos with a current consumption of up to  **1.1 A for slots M1 - M4**  and  **0.75 A for slots M5 and M6**. Cap is compatible with plates:

-   [Arduino Uno / Uno SMD](http://botland.com.pl/arduino-moduly-glowne/1060-arduino-uno-r3.html)
-   [Arduino Uno Wifi](http://botland.com.pl/arduino-moduly-glowne/6244-arduino-uno-wifi.html)
-   [Arduino Due](http://botland.com.pl/arduino-moduly-glowne/1214-arduino-due-arm-cortex.html)
-   [Arduino Mega 2560](http://botland.com.pl/arduino-moduly-glowne/1064-arduino-mega-adk-android-rev3-.html)
-   [Arduino Leonardo](http://botland.com.pl/arduino-moduly-glowne/1213-arduino-leonardo.html)  and  [Leonardo ETH](http://botland.com.pl/arduino-moduly-glowne/3928-arduino-leonardo-ethernet.html)
-   [Arduino M0](http://botland.com.pl/arduino-team-oryginalne-plytki/4541-arduino-m0-32-bit-cortex-m0.html)  and  [Arduino M0 Pro](http://botland.com.pl/arduino-team-oryginalne-plytki/3707-arduino-zero-pro.html)
-   [Arduino Yun](http://botland.com.pl/arduino-moduly-glowne/1859-arduino-yun-wifi-ethernet.html)
-   [Arduino Tian](http://botland.com.pl/arduino-moduly-glowne/6294-arduino-tian-wifi-ethernet-bluetooth.html)


[![Sterownik robota Arduino](http://botland.com.pl/img/art/inne/06362_11b.jpg "Arduino Shield Braccio")](http://botland.com.pl/img/art/inne/06362_11.jpg)

_The driver of the arm, cap over Arduino - Braccio Shield._


**Used I/Os of Arduino**

|I/O of  | Braccio  | | I/O of | Arduino|
| :------|:--:|:--:| :------| -----:|
| **M1** | 11 |   | **I1** | A1 (15) |
| **M2** | 10 |   | **I2** | A2 (16) |
| **M3** | 9 |   | **I3** | A3 (17) |
| **M4** | 6 |   | **I4** | A4 (18) |
| **M5** | 5 |   | **I5** | A5 (19) |
| **M6** | 3 |   | **I2C (TWI)** | SDA, SCL |
| **I0** | A0 (14) |   | **SERIAL** | RX0, TX0 |


#### Installation of arm - video tutorial

The mounting of device can be found in  [the user guide](http://download.arduino.org/products/braccio/Braccio_Quick_Start_Guide.pdf) and video posted below.

#### Specification

-   Supply voltage: 5 V
-   Current consumption: up to 5 A
-   Maximum load capacity: 150g for arm 32 cm
-   Maximum height: 800 mm
-   Maximum width: 520 mm
-   Base width: 140 mm
-   Weight after folding: 792 g
-   **4x [Servo](https://botland.com.pl/en/135-serwomechanizmy)  SR43:**
    -   Control: PWM signal
    -   Torque: 12.2 kg for 4.8 volts, 14.5 kg for 6.0 V
    -   Speed: 0.2 s/60° for 4.8 V, 0,18 sec/60° for 6.0 V
    -   Doubly bearings, metal
    -   Rotational range: 180 °
    -   Connector type: Fatuba
    -   Dimensions: 42,0 x 20.5 x 39.5 mm
    -   Weight: 62 g
-   **2x Servo SR311:**
    -   Control: PWM signal
    -   Torque: 3,1 kg for 4,8 V, 3.8 kg for 6.0 V
    -   Speed: 0.14 sec/60° for 4.8 V, 0.12 seconds/60° 6.0 V
    -   Doubly bearings, metal
    -   Rotational range: 180 °
    -   Connector type: Fatuba
    -   Dimensions: 31,3 x 16.5 x 28.6 mm
    -   Weight: 27 g
-   Power supply: 5 V / 5 A (included)

#### Includes:

-   1x cap over Arduino Braccio Shield (Arduino main module must be purchased separately)
-   1x power adapter 5 V / 5 A
-   6x servos:
    -   2x Servo SR311
    -   4x Servo SR431
-   21 plastic elements
-   1x screwdriver
-   set of screws and nuts
-   1x package

# Schematics

# Code

# Credits