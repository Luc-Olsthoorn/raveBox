#include <Adafruit_NeoPixel.h>
#include "strands.h"
#include <PWMServo.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
//PIN DEFINITIONS


#define  KEYB      1
#define  KEYR      0
#define  SERVO     22
#define  STRANDPIN 23 
const int POTPIN1 = A0;
const int POTPIN2 = A1;

int val;
LEDStrand *strandy = new LEDStrand(STRANDPIN, 30); 
PWMServo myServo;
void setup() {
  Serial.begin(9600);    
  pinMode(KEYR, INPUT); 
  pinMode(KEYB, INPUT); 
  myServo.attach(SERVO);
  attachInterrupt(digitalPinToInterrupt(KEYR), turnOff, RISING);
  attachInterrupt(digitalPinToInterrupt(KEYB), turnOn, RISING);
  
}
void turnOff(){
}
void turnOn(){
}

void loop() {
    val = analogRead(POTPIN1);
    int ServoVal = map(val, 10, 1023, 0, 179); 
    int strandVal = map(val, 0, 1023, 0, 30); 
    myServo.write(ServoVal); 
    strandy->fillFromLeft(strandVal);
    Serial.write(45);
    delay(5);
     
  // Some example procedures showing how to display to the pixels:

  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
  //rainbowCycle(20);
  //theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color

