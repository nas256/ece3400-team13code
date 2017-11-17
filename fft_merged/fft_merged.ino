/*
fft_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

int currtime = 0;
int prevtime = 0;
void setup(){
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe7; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  setup_optical();
  //setup_acoustic();
  //acoustic();
}

void setup_acoustic(){
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe7; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void setup_optical() {
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x41; // use adc0 //40
  DIDR0 = 0x01; // turn off the digital input for adc0 //01
}

void loop(){
  optical();
}

void acoustic() {
  while(1) { // reduces jitter
    
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf7; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    
    Serial.println("start_a");
    /* for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.print(i);
      Serial.print(":");
      Serial.println(fft_log_out[i]); // send out the data
    } */
    
    if (fft_log_out[18] > 150) {
      if (fft_log_out[18] > fft_log_out[16]
          && fft_log_out[18] > fft_log_out[20]) {
      Serial.println("660 detected");
          }
    }
    
    if (Serial.available() && (Serial.read()=='o'|Serial.read()=='O')){
      setup_optical();
      optical();
    }  
  }
}

void optical() {
  while(1) { // reduces jitter
    currtime = millis();
    Serial.println(currtime-prevtime);
    prevtime = currtime;
    cli();  // UDRE interrupt slows this way down on arduino1.0
    
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    
    sei();
    
    Serial.println("start_o");
    /*for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.println(fft_log_out[i]); // send out the data
    }*/
    
    if ( fft_log_out[47] > 100 )
      Serial.println("7kHz beacon detected!");
    else if ( fft_log_out[81] > 100 )
      Serial.println("12kHz beacon detected!");
    else if (fft_log_out[114] > 95 )
      Serial.println("17kHz beacon detected!");
    else Serial.println("Nothing");
      
    /*if (Serial.available() && (Serial.read()=='a'|Serial.read()=='A')){
      setup_acoustic();
      acoustic();;
    }  */
  }
}
