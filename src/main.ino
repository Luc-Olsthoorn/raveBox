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
const int POTPIN4 = A3;
const int POTPIN5 = A4;

int potPin1Val;
int potPin2Val;
int potPin3Val;
int potPin4Val;
int potPin5Val;
int counter =0;
bool on=false;

LEDStrand *strandy = new LEDStrand(STRANDPIN, 30); 
PWMServo myServo;
void setup() {
	

	Serial.begin(9600);    
	pinMode(KEYR, INPUT); 
	pinMode(KEYB, INPUT); 
	myServo.attach(SERVO);
	strandy->color(32);
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
   			strandy->updateLEDS();
		}
		else{
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
  	int mode = map(potPin1Val, 0, 1023, 1, 2);
  	Serial.write("Mode: ");
  	Serial.println(mode);
  	switch(mode){
  		case 1:{
  		 	//-------------------
		  	//Mode 1: Adjust LED and Servo directly
		  	//------------------- 
    		//LED Strand Portion
		  	if(counter%1==0 && on){	
			    strandy->fillFromLeft(map(potPin2Val, 0, 1023, 0, 30));	
			    strandy->setBrightness(map(potPin3Val, 0, 1023, 0, 255));
			    strandy->color(map(potPin2Val, 0, 1023, 0, 360));

				strandy->updateLEDS();
		    }
		    //Servo
		    if(counter%1==0 && on){
		    	//myServo.write(map(potPin5Val, 0, 1023, 0, 179)); 
		    }
		 } break;
		 case 2:{
		 	//-------------------
		  	//Mode 2: Random fun things
		  	//------------------- 
		  	int val = map(potPin3Val, 0, 1023, 1, 500);
		    if(counter%val==0 && on){
		    	
			    //strandy->setBrightness(map(potPin3Val, 0, 1023, 0, 255));
		    	strandy->rainbow();	
		    }
		    if(counter%1==0 && on){
		    	strandy->fillFromLeft(map(potPin2Val, 0, 1023, 0, 30));
		    }	
		    strandy->updateLEDS();
		 }
  	}
  
    
    
    
    


    //This is run everytime
    if(counter == 1000){
    	counter = 0;
    }
    counter++;
    delay(5);
}



