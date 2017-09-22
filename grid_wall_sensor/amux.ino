#include "amux.h"

void amux_init(void){
  // Initializes outputs
  pinMode(AMUX_A, OUTPUT);
  pinMode(AMUX_B, OUTPUT);
  pinMode(AMUX_C, OUTPUT);

  // If using the enable pin, enable it as an output
  // and enable the mux
  #ifdef AMUX_USE_ENABLE
  pinMode(AMUX_EN, OUTPUT);
  amux_enable();
  #endif
  
  amux_select(0); // Start off at first input
}

void amux_select(char mux_input){
  // Update each pin according to the mux_input (0-7)
  digitalWrite(AMUX_A, mux_input & (1<<0) ? HIGH : LOW);
  digitalWrite(AMUX_B, mux_input & (1<<1) ? HIGH : LOW);
  digitalWrite(AMUX_C, mux_input & (1<<2) ? HIGH : LOW);
}

#ifdef USE_ENABLE_PIN

void amux_enable(void){
  digitalWrite(AMUX_A, LOW);
}

void amux_disable(void){
  digitalWrite(AMUX_A, HIGH);
}

#endif
