//Ascii art time
//----A0----
//F5------B1
//----G6----
//E4------C2
//----D3----
//-------DP7
class LEDSegment{
	private:
		int LEDSegmentArray[8];
		int led[8][8]={{1,0,0,1,1,1,1,1},{0,0,1,0,0,1,0,1},{0,0,0,0,1,1,0,1}};
		int last;
	public:
		//store array
		LEDSegment(int arr[]){
			 for (int i =0; i<8; i++){
			 	LEDSegmentArray[i] = arr[i];
			 }
		}
		//initialize pins
		void setup(){
			for(int i=0; i < 8; i++){
				pinMode(LEDSegmentArray[i], OUTPUT);
			}		
		}
		void turnOff(){
			for(int i =0; i<8; i++){
				digitalWrite(LEDSegmentArray[i], HIGH);
			}
			last = 9;
		}
		void change(int in){
			for(int i =0; i<8; i++){
				digitalWrite(LEDSegmentArray[i], led[in][i]);
			}
		}
		void changeNum(int num){
			if(last != num){
				switch(num){
					case 1:
						change(0);
					break;
					case 2: 
						change(1);
					break;
					case 3: 
						change(2);
					break;
				}
			}
			last = num;
			
		}
};