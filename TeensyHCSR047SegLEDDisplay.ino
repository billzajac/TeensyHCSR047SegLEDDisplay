#include <SoftwareSerial.h>

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
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
----
*/
#define trigPin 12
#define echoPin 13


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
  
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // If we want to play with faster communication rate
  //displaySerial.begin (19200);
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
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1; // cm
  // distance = duration/58; // cm

  
  // Vary the blink rate based on the distance measured
  // To keep the 60 ms minimum recommended cycle, we will be able to measure to
  // 3 cm minimum accuracy if we use a multiplier of 10
  if (distance < 30) {
    digitalWrite(led,HIGH);
    delay(10 * distance);
    digitalWrite(led,LOW);
    delay(10 * distance);
  }
  else {
    digitalWrite(led,HIGH);
    delay(300);  // Out of range default blink is 500 ms
    digitalWrite(led,LOW);
    delay(300);  // Out of range default blink is 500 ms
  }  
  
  // Now print the info to the serial and to the display
  //if (distance >= 200 || distance <= 0){
  //  Serial.println("Out of range");
  //  serialDisplay.print("OOR ");
  //}
  //else {
    Serial.print(distance);  
    // This is how to get sprintf behavior: 
    // dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
    // Note: our 7 segment LED display is 4 chars in size
    char buffer[4];
    dtostrf(distance,4,0,buffer);
    serialDisplay.print(buffer);
  //}
}
