#define DEBOUNCE_VARIABLES 2
#define DEBOUNCE_MAXVAL    10

// Debounce variables
#define D_INTERSECT_1 0
#define D_INTERSECT_2 1

extern char deb_array[DEBOUNCE_VARIABLES];

void signal_condition(char debounce_var);
void reset_condition(char debounce_var);
char poll_condition(char debounce_var);

