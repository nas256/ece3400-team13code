#ifndef MAPPER_H
#define MAPPER_H


// Define cardinal directions
#define NORTH  0
#define EAST  1
#define SOUTH 2
#define WEST 3
#define DONE 5

// Define grid sizes
#define GRID_SIZE_X 5
#define GRID_SIZE_Y 4

struct xy_pair{
  uint8_t x;
  uint8_t y;
};
typedef struct xy_pair xy_pair;

#include "stack.h"

struct tile{
     xy_pair xy_pair;
     char traversed;
     char walls; // LSB 4 bits (WEST, EAST, NORTH, SOUTH)
};
typedef struct tile tile;

// Initialize state variables
tile tile_array[GRID_SIZE_X][GRID_SIZE_Y];
extern uint8_t cur_orientation;

extern xy_pair pos;

// Methods
void init_mapper();
void move_to(char x, char y);
uint16_t serialize_tile(tile& tile);

#endif

