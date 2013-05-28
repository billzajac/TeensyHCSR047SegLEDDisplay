/*
Copyright (C) 2013 William Zajac

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
--------------------------------------------------
7 Segment LED display with serial communication
--------------------------------------------------
https://www.sparkfun.com/products/9764 (COM-09764)
https://www.sparkfun.com/datasheets/Components/LED/7-Segment/SFE-0012-DS-7segmentSerial-v41.pdf
This is used to send messages to the 7seg display
Behind the scenes, when this SoftwareSerial object is created it automatically 
sets sigPin (the transmit pin) to OUTPUT mode and writes it HIGH.
This will interfere with INPUT on this pin!  You must be sure to switch this pin
if you want to read from it.
----
VCC -> Arduino 5v
GND -> Arduino GND
RX -> Arduino A0
----
*/
#include <SoftwareSerial.h>
int serialDisplayPin = A0;
SoftwareSerial serialDisplay(1, serialDisplayPin);

/*
--------------------------------------------------
HC-SR04 Ping distance sensor
--------------------------------------------------
http://www.amazon.com/Ultrasonic-Module-HC-SR04-Distance-Arduino/dp/B004U8TOE6
http://www.micropik.com/PDF/HCSR04.pdf
Formula: uS / 58 = centimeters or uS / 148 =inch; or: the 
range = high level time * velocity (340M/S) / 2; we suggest to use over 60ms 
measurement cycle, in order to prevent trigger signal to the echo signal.
----
VCC -> 5v GND 
GND -> COMMON GROUND
Echo -> Arduino 13
Trig -> Arduino 12
----
*/
#include <NewPing.h>
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


/*
--------------------------------------------------
LED
--------------------------------------------------
Pin 13 has an LED connected on most Arduino boards.
Pin 11 has the LED on Teensy 2.0
*/
int led = 11;  // teensy PD6


/* -------------------------------------------------- */
/* Setup                                              */
/* -------------------------------------------------- */
void setup() {
  // initialize the digital pin as an output for the LED
  pinMode(led, OUTPUT);
  
  Serial.begin (115200); // Open serial monitor at 115200 baud to see ping results.
  
  // If we want to play with faster communication rate
  //displaySerial.begin (115200);
  //delay(50);
  //displaySerial.print(B01111111,BYTE);
  //displaySerial.print(B00000100,BYTE);
 
  pinMode(serialDisplayPin, OUTPUT);

  //displaySerial.write(0x7A); // Brightness command bit. Must be followed by:
  //displaySerial.write(0x01); // FF is dimmest, 01 is brightest. Can also use displaySerial.print("z1")
  //displaySerial.print("wB"); // Turn the 2nd dot on. w@ turns all off.

  serialDisplay.begin(9600);
  serialDisplay.print("z");
  delay(100);
  serialDisplay.print(0x00, BYTE);
  delay(100);
  serialDisplay.print("v");
  serialDisplay.print("BOOT");
}


/* -------------------------------------------------- */
/* Main Loop                                          */
/* -------------------------------------------------- */
void loop() {
  // Measure distance with the HC-SR04
  delay(50);                           // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping();      // Send ping, get ping time in microseconds (uS)
  int distance = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm (0 = outside set distance range)
  
  // Vary the blink rate based on the distance measured
  if (distance < 30) {
    blink(10 * distance, 1);
  }
  else {
    blink(300, 1);
  }  
  
  // Now print the info to the serial and to the display
  if (distance == 0){
    Serial.print("Out of range (greater than ");
    Serial.print(MAX_DISTANCE);
    Serial.println("cm)");

    serialDisplay.print("OOR ");
  }
  else {
    Serial.print("Ping: ");
    Serial.print(distance); // (0 = outside set distance range)
    Serial.println("cm");

    // This is how to get sprintf behavior: 
    // dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
    // Note: our 7 segment LED display is 4 chars in size
    char buffer[4];
    dtostrf(distance,4,0,buffer);
    serialDisplay.print(buffer);
  }
}

void blink(int blinkDelay, int blinkTimes) {
  for (int i = 0; i < blinkTimes; i++) {
    digitalWrite(led,HIGH);
    delay(blinkDelay);
    digitalWrite(led,LOW);
    delay(blinkDelay);
  }
}
