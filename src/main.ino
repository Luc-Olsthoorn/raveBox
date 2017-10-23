#include <Adafruit_NeoPixel.h>
#include "strands.h"
#include <math.h>
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
const int POTPIN3 = A2;

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
  strandy->color();
}
void turnOff(){
}
void turnOn(){
}

void loop() {

    val = analogRead(POTPIN1);
    int ServoVal = map(val, 0, 1023, 0, 179); 
    int strandVal = map(val, 0, 1023, 0, 30);
    myServo.write(ServoVal); 
    strandy->fillFromLeft(strandVal);

    val = analogRead(POTPIN2);
    int brightnessVal = map(val, 0, 1023, 0, 255);
    strandy->setBrightness(brightnessVal);

    val = analogRead(POTPIN3);
    int color = map(val, 0, 1023, 0, 20);
    strandy->rainbowAll(color);

    strandy->updateLEDS();
    delay(5);
     
  
}



