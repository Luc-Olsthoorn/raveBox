#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
//PIN DEFINITIONS


#define  KEYB      11
#define  KEYR      12
#define  SERVO     16
#define  STRANDPIN 17 
bool on;
int val;
bool down;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, STRANDPIN, NEO_GRB + NEO_KHZ800);

Servo myServo;
void setup() {
  val=0;
  on=false;
  down=false;
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(KEYR, INPUT); 
  pinMode(KEYB, INPUT); 
  myServo.attach(SERVO);
  attachInterrupt(digitalPinToInterrupt(KEYR), turnOff, RISING);
  attachInterrupt(digitalPinToInterrupt(KEYB), turnOn, RISING);
}
void turnOff(){
  on=false;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
}
void turnOn(){
  on=true;
}

void loop() {
  if(down){
    val--;
  }else{
    val++;
  }
  if(val == 180)
  {
    down = true;
  }else if (val == 0){
    down = false;
  }

  if(on){
     for (int pos = 0; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
    myServo.write(val); 
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    colorWipe(strip.Color(0, 255, 0), 50); // Green
  }
  // Some example procedures showing how to display to the pixels:

  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
  //rainbowCycle(20);
  //theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

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
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
