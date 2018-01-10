class RGBLED {
	private:
		int fade;
		int maxFade;
		int fadeSpeed;
		int red;
		int green;
		int blue;
		int hue;
		int brightness;
		bool on;

	public:
		RGBLED(){
			maxFade =0;
			fade =0;
			fadeSpeed =0;
			red = 0;
			green = 0;
			blue = 0;
			brightness = 0;
			hue = 0;
			bool on = false;
		}
		void turnOn(){
			on = true;
		}	
		//Only overwrites if current fade is halfway done;
		bool setFade(int amount, int speed){
			if(getFade()< 0.5 || maxFade==0){
				maxFade = amount;
				fade = amount;
				fadeSpeed = speed;
				on = true;
				return true;
			}
			return false;
		}
		void stopFade(){
			maxFade =0;
			fade=0;
			fadeSpeed =0;
			turnOff();
		}
		float getFade(){
			if(maxFade==0){
				return 1;
			}
			return ((float)fade/(float)maxFade); 
		}
		void updateFade(){
			
				
			if(fade == 0 && maxFade!=0){
				turnOff();
				maxFade =0;
				fadeSpeed =0; 
			}else if(maxFade!=0){

				fade = fade - fadeSpeed;
				if(fade<0){
					fade =0;
				}
			}
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
				return (red * brightness)/255 * getFade();
			else
				return 0;
		}
		void setRed(int amount){
			red = amount;
		}
		int getGreen(){
			if(on)
				return (green * brightness)/255 * getFade();
			else
				return 0;
		}
		void setGreen(int amount){
			green = amount;
		}
		int getBlue(){
			if(on)
				return (blue *brightness)/255 * getFade();
			else
				return 0;
		}
		void setBlue(int amount){
			blue = amount;
		}
		void setHue(int inHue){
			
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
		float bassLevel = 0.0;
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
			//Serial.write("updating LEDs\n");
			updateAllFade();
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
		void updateAllFade(){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->updateFade();
			}
		}
		void giveFadeAll(int amount, int speed){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setFade(amount, speed);
			}
		}
		void danceLights (bool *frequencyBins, bool beatHit){
			for(int i =0; i < numOfPixels; i++){
				ledArray[i]->setHue(ledArray[i]->getHue()+1);
			}
			//right now assumes boolbox array is the size of LED strand. This can be changed easily with mapping
			
			int frequencyBinsSize = 240;//TODO needs to be abstracted out
			//if(detectBassHit(frequencyBins, frequencyBinsSize)){
			if(beatHit){
				Serial.write("yeet\n");
				
				for(int i= 0;  i< numOfPixels;i++){
					if(frequencyBins[i]){
						
						if(ledArray[i]->setFade(6,1)){
							//ledArray[i]->setHue(ledArray[i]->getHue());
							ledArray[i]->setBrightness(255);
						}
					}else{
						if(ledArray[i]->setFade(6,1)){
							//ledArray[i]->setHue(ledArray[i]->getHue());
							ledArray[i]->setBrightness(255);
						}
					}
				}
				for(int i =0; i < numOfPixels; i++){
					ledArray[i]->setHue(ledArray[i]->getHue()+1);
				}
				//color(0);
				

				

				//Serial.write("\t\tbass hit \n");
				
			}else{
				for(int i= 0;  i< numOfPixels;i++){
					if(frequencyBins[i]){
						
						if(ledArray[i]->setFade(2,1)){
							//ledArray[i]->setHue(ledArray[i]->getHue());
							ledArray[i]->setBrightness(255);
						}
					}
				}
				/*
				int bottom = frequencyBinsSize*.05;
				int top = frequencyBinsSize*.2;
				int total = top - bottom;

				int binSize = numOfPixels/total; 
				
				for(int i= 0;  i< numOfPixels;i++){
					if(frequencyBins[(i/binSize)+bottom]){
						
						if(ledArray[i]->setFade(2,1)){
							ledArray[i]->setHue(i*360/numOfPixels);
							ledArray[i]->setBrightness(50);
						}
					}	
				}*/

			}
		}
		/*
		bool detectBassHit(bool *frequencyBins, int frequencyBinsSize){
			int count=0;

			for(int i =0; i< frequencyBinsSize*.05;i++){
				if(frequencyBins[i]){
					count++;
				}
			}
			
			if (bassLevel < count-2 && frequencyBinsSize*.04 < count){ //-floor(frequencyBinsSize*.4)-2 && floor(frequencyBinsSize*.4)-1 < count){
				bassLevel = count;
				return true;
			}else{

				bassLevel = bassLevel-1;
				if(bassLevel<0){
					bassLevel = 0;
				}
				return false;
			}
		}
	*/
		
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


