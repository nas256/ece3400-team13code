#include "mapper.h"

char current_x, current_y;

void init_mapper(){
  // Start in top right corner
  pos_x = 0;
  pos_y = 0;

  // Start facing NORTH
  cur_orientation = NORTH;

  // Reset maze tiles
  for (int x = 0; x < GRID_SIZE_X; x++){
    for (int y = 0; x < GRID_SIZE_Y; y++){
       grid_array[pos_x][pos_y].traversed = 0;
       grid_array[pos_x][pos_y].walls = 0;
    }
  }
  
}

void move_to(char x, char y){
  grid_array[pos_x][pos_y].traversed = 1;
  pos_x = x;
  pos_y = y;
}

uint16_t serialize_tile(struct grid& grid){
  return 0; // TODO: Implement
}

