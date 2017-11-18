#include "mapper.h"
#include "nRF24L01.h"
#include "RF24.h"
#include "wireless1.h"

xy_pair pos;
uint8_t cur_orientation;
uint8_t mapper_done_flag = 0;

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
       tile_array[x][y].data = (x << 13) | (y << 11);
    }
  }

  // Init DFS stacks
  s_init(&missed_op);
  missed_op.top = -1;
  //Serial.print("M S: ");
  //Serial.println(missed_op.top+1);
  s_init(&visited);
  s_push(&visited, pos);
}

uint8_t true_direction(char wall_direction, char input_robot) {
  uint8_t true_wall_dir = wall_direction + input_robot;
  if (true_wall_dir > 3) true_wall_dir -=4;
  return true_wall_dir;
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
  //Serial.print("Orienting from ");
  print_xy(xy_start);
  //Serial.print("  to  ");
  print_xy(xy_end);
  //Serial.println(" ");
  
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

    //Serial.print("M B: ");
    //Serial.println(missed_op.top+1);
  
  if ( !wall_left  && !s_contains(&visited, left) && !s_contains(&missed_op, left) ){
    //Serial.println("Add left");
    s_push( &missed_op, left ); 
  }

  if ( !wall_right && !s_contains(&visited, right) && !s_contains(&missed_op, right) ){
    //Serial.println("Add right");
    s_push( &missed_op, right ); 
  }
  
  if ( !wall_front && !s_contains(&visited, front) && !s_contains(&missed_op, front) ){
    //Serial.println("Add front");
    s_push( &missed_op, front);  
  }

  unsigned int new_data;
  char true_wall_front = true_direction(cur_orientation, NORTH);
  char true_wall_left =  true_direction(cur_orientation, WEST);
  char true_wall_right = true_direction(cur_orientation, EAST);
  char true_wall_behind = true_direction(cur_orientation, SOUTH);
  char walls = wall_front << true_wall_front | wall_left << true_wall_left | wall_right << true_wall_right;

  tile_set_traversed( pos );
  tile_set_walls( pos, walls );
  tile_transmit( pos );
  
  xy_pair target;
  // Check if we're surrounded by walls or visited
  if ( (wall_front || s_contains(&visited, front)) 
    && (wall_left  || s_contains(&visited,left))
    && (wall_right || s_contains(&visited, right)) ){
      if (s_isEmpty(&missed_op)) return 255;
    target = s_pop(&path);
    //Serial.print("P S: ");
    //Serial.println(path.top+1);
  }else{
    // Not surrounded
    if (s_isEmpty(&missed_op)) return 255;
    target = s_pop(&missed_op);
    s_push(&visited, target);
    //Serial.print("M S: ");
    //Serial.println(missed_op.top+1);
    s_push(&path, pos);
  }
  
  /*new_data = target.x << 13 | target.y << 11 | walls << 3 | 1 << 2 | 1 << 1;
  tile_array[target.x][target.y].walls = walls;
  tile_array[target.x][target.y].traversed = 1;
  
  wireless_send(&new_data, sizeof(unsigned int));
  //need to clear previous current position bit*/
    
  // Find orientation of target relative to pos
  uint8_t orientation = get_orientation(pos, target);
  //if (orientation > WEST) {Serial.print("PROBLEM: "); Serial.println(orientation); }
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
    case DONE:
      break;
  }

  //Serial.print("Moving true: ");
  //Serial.println(orientation);

  //Serial.print("Moving robot: ");
  //Serial.print(cur_orientation);
  //Serial.print("; ");
  //Serial.println(robot_orientation);

  return robot_orientation;
}

void move_to(char x, char y){
  pos.x = x;
  pos.y = y;
  tile_set_traversed(pos);
}

///// UTILITY FUNCTIONS FOR UPDATING TILES ////

void tile_set_traversed(xy_pair xy){
  tile_array[xy.x][xy.y].data |= 1 << 2;
}

void tile_set_walls(xy_pair xy, char walls){
  tile_array[xy.x][xy.y].data |= (walls & 0x7) << 3;
}

void tile_set_ir(xy_pair xy, char freq){
  tile_array[xy.x][xy.y].data |= (freq & 0x3) << 9;
}

void tile_transmit(xy_pair xy){
  uint16_t to_send = tile_array[xy.x][xy.y].data | mapper_done_flag;
  wireless_send ( &to_send, sizeof( uint16_t ) );
  Serial.print("Sent Data: ");
  Serial.println(tile_array[xy.x][xy.y].data);
}

void mapper_done(){
  mapper_done_flag = 1;
}

///////////////////////////////////////////////



