
// Define cardinal directions
#define NONE  0
#define WEST  1
#define EAST  2
#define NORTH 3
#define SOUTH 4

// Define grid sizes
#define GRID_SIZE_X 5
#define GRID_SIZE_Y 4

struct grid{
     char traversed;
     char walls; // LSB 4 bits (WEST, EAST, NORTH, SOUTH)
};
typedef struct grid grid;


// Initialize state variables
grid grid_array[GRID_SIZE_X][GRID_SIZE_Y];
char cur_orientation;

extern char pos_x, pos_y;

// Methods
void init_mapper();
void move_to(char x, char y);
uint16_t serialize_tile(grid& grid);


