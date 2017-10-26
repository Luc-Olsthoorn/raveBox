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
		void color(int val){
	
			
			hsv d = {};
				d.h=val;
				d.s=1; 
				d.v=1;
			rgb a = hsv2rgb(d);
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setBlue(a.b*255);
				ledArray[i]->setRed(a.r*255);
				ledArray[i]->setGreen(a.g*255);
			}
		}
		void rainbow(int val, int i)
		{
			ledArray[i]->setRed(12);
			ledArray[i]->setGreen(3);
			ledArray[i]->setBlue(4);
		}
		void rainbowAll(int val){
			int value =val;
			for(int i =0; i < numOfPixels; i++){
				rainbow(value, i);
			}
		}
};


