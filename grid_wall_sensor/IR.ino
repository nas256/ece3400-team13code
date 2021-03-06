#include "IR.h"

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library
#include "amux.h"

uint8_t old_ADCSRA;

void IR_init(){
  //TIMSK0 = 0; // turn off timer0 for lower jitter
  //ADCSRA = 0xe5; // set the adc to free running mode
  //ADMUX = 0x44; // use adc4 with mux
  //DIDR0 = 0x01; // turn off the digital input for adc0
}

char IR_poll(uint8_t sensor){

    old_ADCSRA = ADCSRA;
    ADCSRA = 0xe5; // free running mode
    ADMUX = 0x44; // use adc4 with mux
  
    amux_select(sensor);
    delayMicroseconds(10);
    
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
    
    /*Serial.println("start");
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.println(fft_log_out[i]); // send out the data
    }*/

    ADCSRA = old_ADCSRA;

    /*for (int i = 110; i < 120; i++){
      Serial.print(i);
      Serial.print(": ");
      Serial.print( fft_log_out[i] );
      Serial.print(", ");
    }
    Serial.println(" ");*/
    
    if ( fft_log_out[49] > 90 ) //120
      return 1;
    else if ( fft_log_out[80] > 70 ) //110
      return 2;
    else if (fft_log_out[114] > 65 ) //100
      return 3;
    else return 0;


  
}

