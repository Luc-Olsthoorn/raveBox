#include <Adafruit_NeoPixel.h>
#include "rgbhsv.h"
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
}


void loop() {
    if(digitalRead(KEYB)==HIGH){
      Serial.write("ON");
      val = analogRead(POTPIN1);
      int ServoVal = map(val, 0, 1023, 0, 179); 
      int strandVal = map(val, 0, 1023, 0, 30);
      myServo.write(ServoVal); 
      strandy->fillFromLeft(strandVal);

      val = analogRead(POTPIN2);
      int brightnessVal = map(val, 0, 1023, 0, 255);
      strandy->setBrightness(brightnessVal);

      int val = analogRead(POTPIN3);
      int color = map(val, 0, 1023, 0, 360);
      strandy->color(color);

      strandy->updateLEDS();
      delay(5);    
    }else{
      Serial.write("OFF");
      strandy->setBrightness(0);
      strandy->updateLEDS();
      delay(100);  
    }
}



