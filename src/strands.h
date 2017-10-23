class RGBLED {
	private:
		int red;
		int green;
		int blue;
		int brightness;
		bool on;

	public:
		RGBLED(){
			red=0;
			green=0;
			blue=0;
			brightness=0;
			bool on = false;
		}
		void turnOn(){
			on = true;
		}	
		void turnOff(){
			on = false;
		}
		void setBrightness(int amount){
			brightness = amount;
		}

		int getRed(){
			if(on)
				return red * brightness/255;
			else
				return 0;
		}
		void setRed(int amount){
			red = amount;
		}
		int getGreen(){
			if(on)
				return green * brightness/255;
			else
				return 0;
		}
		void setGreen(int amount){
			green = amount;
		}
		int getBlue(){
			if(on)
				return blue * brightness/255;
			else
				return 0;
		}
		void setBlue(int amount){
			blue = amount;
		}
};
class LEDStrand {
	private: 
		int PIN;
		int numOfPixels;
		int mode;
		Adafruit_NeoPixel strip; //Actually controls the lights
		RGBLED **ledArray; 

	public:
		LEDStrand(int inPIN, int inNumOfPixels){
			PIN = inPIN;
			numOfPixels = inNumOfPixels;

			mode=0;
			ledArray =  new RGBLED*[numOfPixels];
			for(int i=0; i<inNumOfPixels; i++){
				ledArray[i] = new RGBLED();
			}
			strip = Adafruit_NeoPixel(numOfPixels, inPIN, NEO_GRB + NEO_KHZ800);
			strip.begin();
  			strip.show();
		}
		void updateMode(){
			switch(mode){
				default: 
					break;
			}
		}
		void setMode(int inMode){
			mode = inMode;
		}
		void updateLEDS(){
			for(int i=0; i<numOfPixels; i++){
				strip.setPixelColor(i, strip.Color(ledArray[i]->getRed(), ledArray[i]->getGreen() , ledArray[i]->getBlue()));
			}
			strip.show();
		}
		void setBrightness(int val){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setBrightness(val);
			}
		}
		void fillFromLeft(int val){
			if(val>numOfPixels){
				val = numOfPixels;
			}
			for(int i =0; i < val; i++){
				ledArray[i]->turnOn();
			}
			for(int i=val; i<numOfPixels; i++){
				ledArray[i]->turnOff();
			}
			strip.show();
		}
		void color(){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setBlue(255);
			}
		}
		void rainbow(int val, int i)
		{
			int frequency1 = .3;
			int frequency2 = .3;
			int frequency3 = .3;
			int center = 128;
			int width = 127;
			int phase1=0;
			int phase2=2;
			int phase3=4;
			ledArray[i]->setRed((int)(sin(frequency1*val + phase1) * width + center));
			ledArray[i]->setGreen((int)(sin(frequency2*val + phase2) * width + center));
			ledArray[i]->setBlue((int)(sin(frequency3*val + phase3) * width + center));
			
		}
		void rainbowAll(int val){
			int value =val;
			for(int i =0; i < numOfPixels; i++){
				rainbow(value, i);
			}
		}
		/*
		void rainbow(uint8_t wait) {
		  uint16_t i, j;

		  for(j=0; j<256; j++) {
		    for(i=0; i<strip.numPixels(); i++) {
		      strip.setPixelColor(i, Wheel((i+j) & 255));
		    }
		    strip.show();
		    delay(wait);
		  }
		}

		// Slightly different, this makes the rainbow equally distributed throughout
		void rainbowCycle(uint8_t wait) {
		  uint16_t i, j;

		  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
		    for(i=0; i< strip.numPixels(); i++) {
		      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
		    }
		    strip.show();
		    delay(wait);
		  }
		}

		//Theatre-style crawling lights.
		void theaterChase(uint32_t c, uint8_t wait) {
		  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
		    for (int q=0; q < 3; q++) {
		      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
		        strip.setPixelColor(i+q, c);    //turn every third pixel on
		      }
		      strip.show();

		      delay(wait);

		      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
		        strip.setPixelColor(i+q, 0);        //turn every third pixel off
		      }
		    }
		  }
		}

		//Theatre-style crawling lights with rainbow effect
		void theaterChaseRainbow(uint8_t wait) {
		  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
		    for (int q=0; q < 3; q++) {
		      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
		        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
		      }
		      strip.show();

		      delay(wait);

		      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
		        strip.setPixelColor(i+q, 0);        //turn every third pixel off
		      }
		    }
		  }
		}

		// Input a value 0 to 255 to get a color value.
		// The colours are a transition r - g - b - back to r.
		
		*/
};


