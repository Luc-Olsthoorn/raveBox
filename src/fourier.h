
class audioSampler{
  private:
    static const int PEAK_SAMPLES = 100; // Number of peaks to buffer for scaling
    static const int DC_OFFSET = 512; // DC offset
    static const int SAMPLE_RATE_HZ = 9000;                        // Sample rate of the audio in hertz
    static const int DB_BRIGHT = 65;
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
    float peakSamples[PEAK_SAMPLES]; // Peak samples for ADC scaling
    int n1 = 0; int n2 = 0;
    int sampleCounter = 0;
    char commandBuffer[MAX_CHARS];
    float *frequencyWindow; //here to reference within class scope
    int pixelBoxesCount;
    int audioInputPin;
    int peakSample =0;
    int peakSampleCounter = 0; // Counter for peak sample array

  public:
    audioSampler(int audioInputPinIn, int pixelBoxes){
        audioInputPin = audioInputPinIn;
        pixelBoxesCount = pixelBoxes; 
        frequencyWindow = new float[pixelBoxes+1]; //actually set size 

        pinMode(audioInputPin, INPUT);            // Sets pin for reading input signal voltage 
        analogReadResolution(ANALOG_READ_RESOLUTION);     // Sets bit resolution for voltage to bit mapping
        analogReadAveraging(ANALOG_READ_AVERAGING);       // Sets the number of samples to take for each analogRead call
    }
    void runFFT(){
      arm_cfft_radix4_instance_f32 fft_inst;          // Initialize the FFT
      arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);  // Set size of FFT
      arm_cfft_radix4_f32(&fft_inst, samples);          // Run the FFT
      arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);     // Calculate magnitude of complex numbers output by the FFT
    }
    void endOfSampleCollection(){
      scaleSamples(findPeakVolume());
      runFFT();

    }
    void getBoolBox(bool *boolBox){
      //pass in boolbox and fill that bitch
      for (int i = 0; i < pixelBoxesCount; i++) {    
          if (20.0*log10(magnitudes[i])  >= DB_BRIGHT) { boolBox[i] = true; } 
          else { boolBox[i] = false; }
       }
    }
      //returns refrerence to array of intensity in bins between 0 and 1;

    // Complex FFT functions require a coefficient for the imaginary part of the input
    // Since we only have real data, set this coefficient to zero
    void samplingCallback() {
      samples[sampleCounter] = (float32_t)analogRead(audioInputPin);  // Read from the ADC and store the sample data
      samples[sampleCounter+1] = 0.0;
      sampleCounter += 2;                         // Update sample buffer position and stop after the buffer is filled
      if (sampleCounter >= FFT_SIZE*2) {
        sampleCounter = 0;
        endOfSampleCollection();

      }
    }
    float findPeakVolume() {
      float peakSample = DC_OFFSET; // Set peak to midrange of ADC initially
      for (int i = 0; i < FFT_SIZE*2; i+=2) {
        if (samples[i] > peakSample) {
          peakSample = samples[i]; // Find peak
        }
      }

      // Add current peak sample to array
      peakSamples[peakSampleCounter] = peakSample;
      peakSampleCounter++;

      // Reset counter when end of array is reached
      if (peakSampleCounter == PEAK_SAMPLES) {
        peakSampleCounter = 0;
      }

      // Find peak sample of buffered peak samples
      float peakVolume = 0;
      for (int i = 0; i < PEAK_SAMPLES; i++) {
        if (peakSamples[i] > peakVolume) {
          peakVolume = peakSamples[i]; // Find volume
        }
      }
      
      return peakVolume; // Return peak volume
    }


    // Scale samples according to current peak, only apply to even indices
    void scaleSamples(float peakVolume) {
      for (int i = 0; i < FFT_SIZE*2; i+=2) {
        samples[i] = (samples[i] - DC_OFFSET)*(DC_OFFSET/(peakVolume - DC_OFFSET)) + DC_OFFSET; // Scale samples
        Serial.println(samples[i]);
      }
}

};
