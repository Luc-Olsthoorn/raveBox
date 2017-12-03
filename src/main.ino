#include <Adafruit_NeoPixel.h>
#define ARM_MATH_CM4 //for fourier 
#include <arm_math.h> //for fourier 
#include <math.h>
#include "rgbhsv.h"
#include "strands.h"
#include "led7Segment.h"
#include <PWMServo.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
//PIN DEFINITIONS



#define  STRANDPIN 23 
#define  AUDIOINPIN 13 
int counter =0;
bool on=true;

LEDStrand *strandy;
audioSampler *sampler;
void setup() {
	strandy = new LEDStrand(STRANDPIN, 240); 
	sampler = new audioSampler(240); 
	strandy->color(32);
}
void loop() {
	//-------------------
	//Input Portion
	//-------------------
    
    //General info
  	int mode = 1;
  	
  	switch(mode){
  		case 1:{

  		 	//-------------------
		  	//Mode 1: Adjust LED and Servo directly
		  	//------------------- 
    		//LED Strand Portion
		  	if(counter%1==0 && on){	
		  		sampler->sample();
		  		sampler->runFFT();

		  		strandy->danceLights(sampler->getBoolBox());
				strandy->updateLEDS();
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



