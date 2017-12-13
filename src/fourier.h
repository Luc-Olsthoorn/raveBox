
class audioSampler{
  private:
    static const int SAMPLE_RATE_HZ = 9000;                        // Sample rate of the audio in hertz
    static const int DB_BRIGHT = 72.0;
    static const int FFT_SIZE = 256;                       // Size of the FFT; realistically can only be at most 256 
                                  // without running out of memory for buffers and other state
    static const int ANALOG_READ_RESOLUTION = 10;            // Bits of resolution for the ADC
    static const int ANALOG_READ_AVERAGING = 16;             // Number of samples to average with each ADC reading
    static const int MAX_CHARS = 65;                         // Max size of the input command buffer
    float intensitySamples1[10] = {};             // Array for averaging the intensity 
    float intensitySamples2[1000] = {};             // Array for averaging the intensity samples
    float samples[FFT_SIZE*2];
    float oldSamples[FFT_SIZE*2];           
    float magnitudes[FFT_SIZE];
    int n1 = 0; int n2 = 0;
    int sampleCounter = 0;
    char commandBuffer[MAX_CHARS];
    float *frequencyWindow; //here to reference within class scope
    bool *boolBox;
    int pixelBoxesCount;
    int audioInputPin;

  public:
    audioSampler(int audioInputPinIn, int pixelBoxes){
        audioInputPin = audioInputPinIn;
        pixelBoxesCount = pixelBoxes; 
        frequencyWindow = new float[pixelBoxes+1]; //actually set size 
        boolBox = new bool [pixelBoxes];
        pinMode(audioInputPin, INPUT);            // Sets pin for reading input signal voltage 
        analogReadResolution(ANALOG_READ_RESOLUTION);     // Sets bit resolution for voltage to bit mapping
        analogReadAveraging(ANALOG_READ_AVERAGING);       // Sets the number of samples to take for each analogRead call
    }
    void startSampleing(){
      
    }
    void runFFT(){
      copyToOld();
      arm_cfft_radix4_instance_f32 fft_inst;          // Initialize the FFT
      arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);  // Set size of FFT
      arm_cfft_radix4_f32(&fft_inst, oldSamples);          // Run the FFT
      arm_cmplx_mag_f32(oldSamples, magnitudes, FFT_SIZE);     // Calculate magnitude of complex numbers output by the FFT
      convertMagnitudeToBool();
    }
    void convertMagnitudeToBool(){
      for (int i = 0; i < pixelBoxesCount; i+=2) {    
        magnitudes[i+1] = 20.0*log10(magnitudes[i+1]);                      // Convert magnitudes to decibels   
        if (magnitudes[i+1] >= DB_BRIGHT) { boolBox[i+1] = true; } 
        else { boolBox[i+1] = false; }
     }
    }
    bool *getBoolBox(){
      //returns refrerence to array of intensity in bins between 0 and 1;
      return boolBox;
    }
    void copyToOld(){
      for(int i =0; i < pixelBoxesCount; i++){
        oldSamples[i]= samples[i]; 
      }
    }
    // Complex FFT functions require a coefficient for the imaginary part of the input
    // Since we only have real data, set this coefficient to zero
    void samplingCallback() {
      samples[sampleCounter] = (float32_t)analogRead(audioInputPin);  // Read from the ADC and store the sample data
      samples[sampleCounter+1] = 0.0;
      sampleCounter += 2;                         // Update sample buffer position and stop after the buffer is filled
      if (sampleCounter >= FFT_SIZE*2) {
        sampleCounter = 0;
        runFFT();

      }
    }
};
