## 7 Segment LED display with serial communication
* https://www.sparkfun.com/products/9764 (COM-09764)
* https://www.sparkfun.com/datasheets/Components/LED/7-Segment/SFE-0012-DS-7segmentSerial-v41.pdf

This is used to send messages to the 7seg display
Behind the scenes, when this SoftwareSerial object is created it automatically
sets sigPin (the transmit pin) to OUTPUT mode and writes it HIGH.
This will interfere with INPUT on this pin!  You must be sure to switch this pin if you want to read from it.

### Wiring
* VCC -> Arduino 5v
* GND -> Arduino GND
* RX -> Arduino A0

## HC-SR04 Ping distance sensor
* http://www.amazon.com/Ultrasonic-Module-HC-SR04-Distance-Arduino/dp/B004U8TOE6
* http://www.micropik.com/PDF/HCSR04.pdf
Formula: uS / 58 = centimeters or uS / 148 =inch; or: the range = high level time * velocity (340M/S) / 2; we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.

### Wiring
* VCC -> 5v GND
* GND -> COMMON GROUND
* Echo -> Arduino 13
* Trig -> Arduino 12

## LED
* Pin 13 has an LED connected on most Arduino boards.
* Pin 11 has the LED on Teensy 2.0
