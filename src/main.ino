#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "rgbhsv.h"
#include "strands.h"
#include "led7Segment.h"
#include <PWMServo.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
//PIN DEFINITIONS


#define  KEYB      1
#define  KEYR      0
#define  SERVOCONTROLPIN 	21
#define  SERVO     22
#define  STRANDPIN 23 
//5-12 reserved for segment
int segmentArr[8] = {5,6,7,8,9,10,11,12};
const int POTPIN1 = A0;
const int POTPIN2 = A3;
const int POTPIN3 = A1;
const int POTPIN4 = A2;
const int POTPIN5 = A4;

int potPin1Val;
int potPin2Val;
int potPin3Val;
int potPin4Val;
int potPin5Val;
int counter =0;
bool on=false;

LEDStrand *strandy = new LEDStrand(STRANDPIN, 30); 
LEDSegment *segmenty = new LEDSegment(segmentArr);
PWMServo myServo;
void setup() {
	pinMode(SERVOCONTROLPIN, OUTPUT);
	Serial.begin(9600);    
	pinMode(KEYR, INPUT); 
	pinMode(KEYB, INPUT); 
	myServo.attach(SERVO);
	
	strandy->color(32);
	segmenty->setup();
	segmenty->turnOff();
}


void loop() {
	//-------------------
	//Input Portion
	//-------------------

    //This is for being on
    if(counter%1==0){
    	if(digitalRead(KEYB)!=HIGH){
    		on=false;
    		strandy->setBrightness(0);
    		segmenty->turnOff();
   			strandy->updateLEDS();
   			digitalWrite(SERVO, LOW);
   			digitalWrite(SERVOCONTROLPIN, LOW);
		}
		else{
			digitalWrite(SERVOCONTROLPIN, HIGH);
    		on=true;
    	}
    }
	
    //This reads in multiple Pot values
    if(counter%1==0 && on){
	    potPin1Val = analogRead(POTPIN1);
	    potPin2Val = analogRead(POTPIN2);
	    potPin3Val = analogRead(POTPIN3);
	    potPin4Val = analogRead(POTPIN4);
	    potPin5Val = analogRead(POTPIN5);
    }
    
    //General info
  	int mode = map(potPin1Val, 0, 1023, 1, 3);
  	
  	switch(mode){
  		case 1:{
  			if(on){
  				segmenty->changeNum(1);
  			}
  		 	//-------------------
		  	//Mode 1: Adjust LED and Servo directly
		  	//------------------- 
    		//LED Strand Portion
		  	if(counter%1==0 && on){	
		  		strandy->setBrightness(map(potPin2Val, 0, 1023, 0, 255));
			    strandy->fillFromLeft(map(potPin3Val, 0, 1023, 0, 30));	
			    strandy->color(map(potPin4Val, 0, 1023, 0, 360));

				strandy->updateLEDS();
		    }
		    
		    //Servo
		    if(counter%1==0 && on){
		    	myServo.write(map(potPin5Val, 0, 1023, 0, 179)); 
		    }
		 } break;
		 case 2:{
		 	if(on){
		 		segmenty->changeNum(2);
		 	}
		 	//-------------------
		  	//Mode 2: Rainbow
		  	//------------------- 
		  	
		    if(counter%1==0 && on){	
		    	strandy->setBrightness(map(potPin2Val, 0, 1023, 0, 255));
		    }
		    int val = map(potPin3Val, 0, 1023, 1, 10);
		    if(counter%val==0 && on){
		    	strandy->rainbow();	
		    }
		    strandy->updateLEDS();
		    
		    //Servo
		    if(counter%1==0 && on){
		
		    	myServo.write(map(potPin5Val, 0, 1023, 0, 179)); 
		    	
		    }
		 } break;
		 case 3:{
		 	if(on){
		 		segmenty->changeNum(3);
		 	}
		 	//-------------------
		  	//Mode 3: Flash
		  	//------------------- 
		  	
		    if(counter%1==0 && on){	
		    	strandy->setBrightness(map(potPin2Val, 0, 1023, 0, 255));
		    	strandy->color(map(potPin4Val, 0, 1023, 0, 360));
		    }
		    int val = map(potPin3Val, 0, 1023, 1, 100);
		    if(counter%(1000/val)==0 && on){
		    	strandy->flash();	
		    }
		    strandy->updateLEDS();
		    
		    //Servo
		    if(counter%1==0 && on){
		    	myServo.write(map(potPin5Val, 0, 1023, 0, 179)); 
		    }
		 } break;
		  case 4:{
		 	if(on){
		 		segmenty->changeNum(3);
		 	}
		 	//-------------------
		  	//Mode 2: Random fun things
		  	//------------------- 
		  	
		    if(counter%1==0 && on){	
		    	strandy->setBrightness(map(potPin2Val, 0, 1023, 0, 255));
		    }
		    int val = map(potPin4Val, 0, 1023, 1, 100);
		    if(counter%(1000/val)==0 && on){
		    	strandy->flash();	
		    }
		    strandy->updateLEDS();
		    
		    //Servo
		    if(counter%1==0 && on){
		    	myServo.write(map(potPin5Val, 0, 1023, 0, 179)); 
		    }
		 } break;
  	}
  	//--------
    //Debuging
    //-------- 
    Serial.write("On: ");
  	Serial.println(on);
  	Serial.write("Mode: ");
  	Serial.println(mode);
    //This is run everytime esentially a clock
    if(counter == 1000){
    	counter = 0;
    }
    counter++;
    delay(1);
}



