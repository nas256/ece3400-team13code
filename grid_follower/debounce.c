#include "debounce.h"

char deb_array[DEBOUNCE_VARIABLES];

void signal_condition(char debounce_var){
  if (deb_array[debounce_var] < DEBOUNCE_MAXVAL)
    deb_array[debounce_var] += 1;
}

void clear_condition(char debounce_var){
  deb_array[debounce_var] = 0;
}

char poll_condition(char debounce_var, char target){
  return deb_array[debounce_var] >= target;
}

