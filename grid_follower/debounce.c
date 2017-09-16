#include "debounce.h"

void signal_condition(char debounce_var){
  if (deb_array[debounce_var] < DEBOUNCE_MAXVAL)
    deb_array[debounce_var]++;
}

void reset_condition(char debounce_var){
  deb_array[debounce_var] = 0;
}

void poll_condition(char debounce_var, char target){
  return deb_array[debounce_var] >= target;
}

