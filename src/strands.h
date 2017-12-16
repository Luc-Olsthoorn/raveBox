class RGBLED {
	private:
		int red;
		int green;
		int blue;
		int hue;
		int brightness;
		bool on;

	public:
		RGBLED(){
			red=0;
			green=0;
			blue=0;
			brightness=0;
			hue=0;
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
		int getBrightness(){
			return brightness;
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
		void setHue(int inHue){
			Serial.println(inHue);
			if(inHue == 360){
				inHue = 0;
			}
			hue = inHue;
			hsv theHSV = {};
			theHSV.h=inHue;
			theHSV.s=1; 
			theHSV.v=1;
			rgb a = hsv2rgb(theHSV);
			red = a.r*255;
			green = a.g*255;
			blue = a.b*255;
		}
		int getHue(){
			return hue;
		}
};
class LEDStrand {
	private: 
		int PIN;
		int state;
		int numOfPixels;
		float bassLevel =0.0;
		Adafruit_NeoPixel strip; //Actually controls the lights
		RGBLED **ledArray; 

	public:
		LEDStrand(int inPIN, int inNumOfPixels){
			PIN = inPIN;
			numOfPixels = inNumOfPixels;

			
			ledArray =  new RGBLED*[numOfPixels];
			for(int i=0; i<inNumOfPixels; i++){
				ledArray[i] = new RGBLED();
			}
			strip = Adafruit_NeoPixel(numOfPixels, inPIN, NEO_GRB + NEO_KHZ800);
			strip.begin();
  			strip.show();
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
		void danceLights (bool *boolBoxArr){
			//right now assumes boolbox array is the size of LED strand. This can be changed easily with mapping
			int count=0;
			for(int i =0; i< numOfPixels/10;i++){
				if(boolBoxArr[i]){
					count++;
				}
			}
			if (bassLevel < count-15){
				bassLevel = count;
			}else{

				bassLevel = bassLevel-2;
				if(bassLevel<0){
					bassLevel = 0;
				}
			}
			fillFromSource(floor((bassLevel/10)*numOfPixels));
			/*
			for(int i = numOfPixels/5; i< numOfPixels;i++){
				if(boolBoxArr[i]){
					ledArray[i]->turnOn();
				}else{
					ledArray[i]->turnOff();
				}
			}
			*/

		}

		void flash(){
			if(state==1){
				state=0; 
				for(int i=0; i<numOfPixels; i++)
				{
					ledArray[i]->turnOff();
				}
			}else{
				for(int i=0; i<numOfPixels; i++)
				{
					ledArray[i]->turnOn();
				}
				state=1;
			}
		}
		void fillFromSource(int val){
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
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setHue(val);
			}
		}
		void rainbow()
		{
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setHue(ledArray[i]->getHue()+1);
			}
		}
		void staticRainbow(){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setHue(i*360/numOfPixels);
			}
		}
		void turnOn(){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->turnOn();
			}
		}
};


