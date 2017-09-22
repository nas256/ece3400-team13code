#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#define DEBOUNCE_VARIABLES 5
#define DEBOUNCE_MAXVAL    40

// Debounce variables
#define D_INTERSECT_1       0
#define D_INTERSECT_2       1
#define D_INTERSECT_1_W     2
#define D_INTERSECT_2_W     3
#define D_WALL_FRONT        4 

//extern char deb_array[DEBOUNCE_VARIABLES];

void signal_condition(char debounce_var);
void clear_condition(char debounce_var);
char poll_condition(char debounce_var, char target);

#endif
