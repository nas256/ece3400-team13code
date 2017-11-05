#include "mapper.h"

xy_pair pos;
uint8_t cur_orientation;

void print_xy(struct xy_pair xy){
  Serial.print(xy.x);
  Serial.print("  ");
  Serial.print(xy.y);
}

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
  s_init(&missed_op);
  s_init(&visited);
  //s_push(&missed_op, pos);
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
    /* case STAY:
      break; */
  }

  return output;
}

// get orientation of [end] relative to [start]
// ASSUMES ONLY ONE TILE APART
uint8_t get_orientation(struct xy_pair xy_start, struct xy_pair xy_end){
  Serial.print("Orienting from ");
  print_xy(xy_start);
  Serial.print("  to  ");
  print_xy(xy_end);
  Serial.println(" ");
  
  if (xy_end.x > xy_start.x) return EAST;
  if (xy_end.x < xy_start.x) return WEST;

  if (xy_end.y > xy_start.y) return NORTH;
  if (xy_end.y < xy_start.y) return SOUTH;

  return 255;
}

uint8_t at_intersection(uint8_t wall_front, uint8_t wall_left, uint8_t wall_right){
  // Need to convert from "Front" to XY
  xy_pair front = translate(cur_orientation, NORTH, pos);
  xy_pair left = translate(cur_orientation, WEST, pos);
  xy_pair right = translate(cur_orientation, EAST, pos);
  xy_pair behind = translate(cur_orientation, SOUTH, pos);
  
  if ( !wall_left  && !s_contains(&visited, left) )
    s_push( &missed_op, left ); 

  //Serial.print("wall_right: "); Serial.println(wall_right);
  //Serial.print("visited: "); Serial.println(s_contains(&visited, right));
  if ( !wall_right && !s_contains(&visited, right) ){
    //Serial.println("Adding right to missed");
    s_push( &missed_op, right ); 
  }
  if ( !wall_front && !s_contains(&visited, front) )
    s_push( &missed_op, front);  
  
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
  if (orientation > WEST) {Serial.print("PROBLEM: "); Serial.println(orientation); }
  uint8_t robot_orientation = (orientation - cur_orientation);  
  if (robot_orientation > 3) robot_orientation += 4;

  cur_orientation = orientation;

  switch(robot_orientation){
    case NORTH:
      pos.x = front.x;
      pos.y = front.y;
      break;
    case SOUTH:
      pos.x = behind.x;
      pos.y = behind.y;
      break;
    case EAST:
      pos.x = right.x;
      pos.y = right.y;
      break;
    case WEST:
      pos.x = left.x;
      pos.y = left.y;
      break;
    /*case STAY:
      break; */
  }

  Serial.print("Moving true: ");
  Serial.println(orientation);

  Serial.print("Moving robot: ");
  Serial.print(cur_orientation);
  Serial.print("; ");
  Serial.println(robot_orientation);

  return robot_orientation;
}

void move_to(char x, char y){
  tile_array[pos.x][pos.y].traversed = 1;
  pos.x = x;
  pos.y = y;
}

uint16_t serialize_tile(struct tile& tile){
  return 0; // TODO: Implement
}


