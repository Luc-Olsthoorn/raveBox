#include <Adafruit_NeoPixel.h>
#define ARM_MATH_CM4 //for fourier 
#include <arm_math.h> //for fourier 
#include <math.h>
#include <vector>
#include "rgbhsv.h"
#include "strands.h"
#include "fourier.h"
#include <PWMServo.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif
//PIN DEFINITIONS


#define POWER_LED_PIN 13 // Output pin for power LED
#define  STRANDPIN 0 
#define  AUDIOINPIN 14 

static const int SAMPLE_RATE_HZ = 9000;  
int counter = 0;
bool on = true;
IntervalTimer samplingTimer; //must be gloval as per teenseys suggestion
LEDStrand *strandy;
audioSampler *sampler;
bool boolBoxCartel [240];
bool powerLEDstate = 0;
void setup() {
  pinMode(POWER_LED_PIN, OUTPUT); // Heartbeat
	strandy = new LEDStrand(STRANDPIN, 240); 
	sampler = new audioSampler(AUDIOINPIN, 240); 
	samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
	strandy->color(32);
  strandy->staticRainbow();
  strandy->setBrightness(255);
  strandy->turnOn();
  strandy->updateLEDS();
}

void loop() {
    heartbeat();

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
          sampler->getBoolBox(boolBoxCartel);

          strandy->rainbow();
		  		strandy->danceLights(boolBoxCartel);
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
    heartbeat();

    delay(1);
}
void heartbeat() {
  digitalWrite(POWER_LED_PIN, powerLEDstate); // Heartbeat
  powerLEDstate = !powerLEDstate; // Flip state
}
//Bullshit code you have to write that is limited by teensey not wanting to run too many sampling timers.
void samplingCallback() {
  sampler->samplingCallback();
}

