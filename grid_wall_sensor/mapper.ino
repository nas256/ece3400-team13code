#include "mapper.h"

xy_pair pos;
char cur_orientation;

void init_mapper(){
  // Start in bottom left corner
  pos.x = 0;
  pos.y = 0;

  // Start facing NORTH
  cur_orientation = NORTH;

  // Reset maze tiles
  for (int x = 0; x < GRID_SIZE_X; x++){
    for (int y = 0; y < GRID_SIZE_Y; y++){
       tile_array[x][y].traversed = 0;
       tile_array[x][y].walls = 0;
    }
  }

  // Init DFS stacks
  s_push(&missed_op, pos);
}

// given robot orientation [robot_direction], will move [start] according to [input_robot]
struct xy_pair translate(char robot_direction, char input_robot, struct xy_pair start){
  // robot direction is current direction of the robot (in robot terms!)
  // input_robot is the the direction RELATIVE to the robot

  // Convert to true N, S, E, W
  uint8_t true_dir = robot_direction + input_robot;
  if (true_dir > 3) true_dir -= 4;  // (cheaper than %4, can't do this for converting back though)

  xy_pair output;
  output.x = start.x;
  output.y = start.y;

  // Use true N, S, E, W to increment/decrement x/y correctly
  switch(true_dir){
    case NORTH:
      output.y += 1;
      break;
    case SOUTH:
      output.y -= 1;
      break;
    case EAST:
      output.x += 1;
      break;
    case WEST:
      output.x -= 1;
      break;
  }

  return output;
}

// get orientation of [end] relative to [start]
// ASSUMES ONLY ONE TILE APART
uint8_t get_orientation(struct xy_pair xy_start, struct xy_pair xy_end){
  if (xy_end.x > xy_start.x) return EAST;
  if (xy_end.x < xy_start.x) return WEST;

  if (xy_end.y > xy_start.y) return NORTH;
  if (xy_end.y < xy_start.y) return SOUTH;

  return 255;
}

void at_intersection(uint8_t wall_front, uint8_t wall_left, uint8_t wall_right){
  // Need to convert from "Front" to XY
  xy_pair front = translate(cur_orientation, NORTH, pos);
  xy_pair left = translate(cur_orientation, WEST, pos);
  xy_pair right = translate(cur_orientation, EAST, pos);
  
  if ( !wall_front)
    s_push( &missed_op, front);  
  if ( !wall_left)
    s_push( &missed_op, left ); 
  if ( !wall_right)
    s_push( &missed_op, right ); 
  
  xy_pair target;

  // Check if we're surrounded by walls or visited
  if ( (wall_front || s_contains(&visited, front)) 
    && (wall_left  || s_contains(&visited,left))
    && (wall_right || s_contains(&visited, right)) ){
    target = s_pop(&path);
  }else{
    // Not surrounded
    target = s_pop(&missed_op);
    s_push(&path, pos);
  }

  // Find orientation of target relative to pos
  uint8_t orientation = get_orientation(pos, target);
  if (orientation > WEST) Serial.println("PROBLEM");
  return (orientation - cur_orientation) % 4;  
}

void move_to(char x, char y){
  tile_array[pos.x][pos.y].traversed = 1;
  pos.x = x;
  pos.y = y;
}

uint16_t serialize_tile(struct tile& tile){
  return 0; // TODO: Implement
}

