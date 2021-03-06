#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "wireless1.h"
#include "IR.h"

// Line following code
// ECE 3400 - Group 13

// Low level: 528
// High Level: 977

#include <Servo.h>

#include "debounce.h"
#include "debounce.c" // i hate you arduino ide trash
#include "mapper.h"

#include "amux.h"

// Pin Definitions
#define P_LINE_SENSOR_1        A0 // Left
#define P_LINE_SENSOR_2        A1 // Right
#define P_INTERSECT_SENSOR_1   A2 // Left
#define P_INTERSECT_SENSOR_2   A3 // Right
#define P_AMUX                 A5 //FIX THIS AND WALL FRONT SENSOR
//#define P_WALL_FRONT           A5

#define P_SERVO_LEFT           5
#define P_SERVO_RIGHT          6

#define P_START_BUTTON         3 

// Light Level Parameters
#define WHITE_VALUE            538  // 700
#define BLACK_VALUE            850
#define LIGHT_TOLERANCE        5

// Define servo tuning parameters
#define LEFT_MAX               180
#define RIGHT_MAX              180
#define LEFT_ZERO              92
#define RIGHT_ZERO             87

// Define operating states
#define FOLLOW_LINE            0
#define TURN_LEFT              1
#define TURN_RIGHT             2
#define TURN_180               3

// Turns
#define LEFT                   0
#define RIGHT                  1

#define WALL_THRESHOLD_SIDE    200
#define WALL_THRESHOLD_FRONT   150 //170

//#define LEFT_LED 8
//#define RIGHT_LED 10
//#define FRONT_LED 9

#define FRONT_WALL_POLL_THRESHOLD 30
#define SIDE_WALL_POLL_THRESHOLD  30
#define SEND_TIMER 1000

unsigned long last_turn_start = 0;
unsigned long last_send_time = 0;
unsigned long last_IR_time = 0;

char send_flag = 0;
char ir_flag = 0;
char ir_done = 0;

Servo servo_left, servo_right;
int line_left_value = 0, line_right_value = 0;
char state = FOLLOW_LINE;
char freq = 0;
int timer = 0;

void setup() {
  Serial.begin(115200); // Begin UART Communication
  SPI.begin();
  // Initialize servo variables
  servo_left.attach(P_SERVO_LEFT);
  servo_right.attach(P_SERVO_RIGHT);

  // Setup Line Sensor Inputs
  pinMode(INPUT, P_LINE_SENSOR_1);
  pinMode(INPUT, P_LINE_SENSOR_2);
  pinMode(INPUT, P_INTERSECT_SENSOR_1);
  pinMode(INPUT, P_INTERSECT_SENSOR_2);
  pinMode(INPUT_PULLUP, P_START_BUTTON);
  digitalWrite(P_START_BUTTON, HIGH);
/*  pinMode(OUTPUT, LEFT_LED);
  pinMode(OUTPUT, RIGHT_LED);
  pinMode(OUTPUT, FRONT_LED);
  digitalWrite(LEFT_LED,LOW);
  digitalWrite(RIGHT_LED,LOW);
  digitalWrite(FRONT_LED,LOW);*/

  wireless_setup(1);
  IR_init();
  init_mapper();

  // Initialize the analog mux
  amux_init();

  drive(LEFT_ZERO, RIGHT_ZERO);

  for (uint8_t i = 0; i < 50; i++) Serial.println(" ");

  delay(100);
  wait_start();
  Serial.println("start");

}

int slowdown_left = 0, slowdown_right = 0;

void sense_walls(){
  amux_select(WALL_SENSOR_RIGHT);
  delayMicroseconds(15);
  if ( analogRead(P_AMUX) > WALL_THRESHOLD_SIDE)
     signal_condition(D_WALL_RIGHT);
  else
     clear_condition(D_WALL_RIGHT);

  amux_select(WALL_SENSOR_LEFT);
  delayMicroseconds(15);
  if ( analogRead(P_AMUX) > WALL_THRESHOLD_SIDE)
     signal_condition(D_WALL_LEFT);
  else
     clear_condition(D_WALL_LEFT);
  //Serial.println(analogRead(P_AMUX));

  amux_select(WALL_SENSOR_FRONT);
  delayMicroseconds(15);
  if ( analogRead(P_AMUX) > WALL_THRESHOLD_FRONT)
     signal_condition(D_WALL_FRONT);
  else
     clear_condition(D_WALL_FRONT); 
}

void wait_start(){
  Serial.println(digitalRead(P_START_BUTTON));
  while (digitalRead(P_START_BUTTON)) {}
}

void sense_ir(){
  uint8_t ir1 = IR_poll(AMUX_TREASURE_1);
  uint8_t ir2 = IR_poll(AMUX_TREASURE_2);
  /*Serial.print("IR: ");
  Serial.print( ir1 );
  Serial.print(" | ");
  Serial.println(ir2);*/
  
  tile_set_ir(prev_pos, ir1);
  tile_set_ir(prev_pos, ir2);
}

void do_ir(){
  if (millis() - last_IR_time > 300){
    sense_ir();
    last_IR_time = millis();
  } 
}

void loop() { 
  /* int i = 0;
  while (1) {
    wireless_send(&i, sizeof(int));
    i++;
  } */

  if ( millis() - last_turn_start > 15 && ir_flag < 1){
    sense_ir();
    ir_flag++;
    if (ir_flag >= 1) ir_done = 1;
  }

  // Update readingss from line sensors
  line_left_value = analogRead(P_LINE_SENSOR_1);  // 0-1023
  line_right_value = analogRead(P_LINE_SENSOR_2); // 0-1023


  // Sense intersections
   if ( analogRead(P_INTERSECT_SENSOR_1) > BLACK_VALUE - 40 )
     signal_condition(D_INTERSECT_1);
   else
     clear_condition(D_INTERSECT_1);
     
   if ( analogRead(P_INTERSECT_SENSOR_2) > BLACK_VALUE - 40 )
     signal_condition(D_INTERSECT_2);
   else
     clear_condition(D_INTERSECT_2);
     
   if ( analogRead(P_INTERSECT_SENSOR_1) < WHITE_VALUE + 30 )
     signal_condition(D_INTERSECT_1_W);
   else
     clear_condition(D_INTERSECT_1_W);
     
   if ( analogRead(P_INTERSECT_SENSOR_2) < WHITE_VALUE + 30 )
     signal_condition(D_INTERSECT_2_W);
   else
     clear_condition(D_INTERSECT_2_W);

  // Read wall values
  sense_walls();  
  
  if (state == FOLLOW_LINE){ 
    // Line following adjustments
    if ( abs(line_left_value - line_right_value) < LIGHT_TOLERANCE ){
      // drive straight
      slowdown_left = slowdown_right = 0;
    }else if (line_left_value > line_right_value){
      // turn left
      slowdown_left = map(line_left_value-line_right_value, 0, BLACK_VALUE-WHITE_VALUE, 0, 20);
      slowdown_right = 0;
    }else if (line_left_value < line_right_value){
      // turn right
      slowdown_left = 0;
      slowdown_right = map(line_right_value-line_left_value, 0, BLACK_VALUE-WHITE_VALUE, 0, 20);
    }
  
  
   if ( poll_condition(D_INTERSECT_1, DEBOUNCE_MAXVAL)
          && poll_condition(D_INTERSECT_2, DEBOUNCE_MAXVAL) ){
      // intersection reached

      if ( millis() - last_turn_start > 1000 ){
       uint8_t to_turn = at_intersection( poll_condition(D_WALL_FRONT, FRONT_WALL_POLL_THRESHOLD), 
                                          poll_condition(D_WALL_LEFT, SIDE_WALL_POLL_THRESHOLD),
                                          poll_condition(D_WALL_RIGHT, SIDE_WALL_POLL_THRESHOLD) );
       
       
       if (to_turn == 255) {
        mapper_done();
        tile_transmit( pos );
        while(1) {
          /*digitalWrite(FRONT_LED, HIGH);
          digitalWrite(LEFT_LED, HIGH);
          digitalWrite(RIGHT_LED, HIGH);*/
          drive(LEFT_ZERO, RIGHT_ZERO);
          
        }
       }
       switch(to_turn){
         case NORTH: state = FOLLOW_LINE; break; 
         case SOUTH: state = TURN_180;    break;
         case WEST:  state = TURN_LEFT;   break;
         case EAST:  state = TURN_RIGHT;  break;
       } 
       last_turn_start = millis();
       ir_flag = 0;
       
       last_send_time = millis();
       send_flag = 1;

       //Serial.print("WANT TO TURN "); Serial.println(to_turn);

       
        
      }
    }
  } else if (state == TURN_180){
    // Turn right until right intersection sensor sees white
    slowdown_left = 0;
    slowdown_right = 14;

    if ( millis() - last_turn_start > 1350 )
      state = FOLLOW_LINE;
         
  
  } else if (state == TURN_RIGHT){
    // Turn right until right intersection sensor sees white
    slowdown_left = 0;
    slowdown_right = 7;
    if ( millis() - last_turn_start > 1250 ){
      state = FOLLOW_LINE;
    }
    //do_ir();
  } else if (state == TURN_LEFT){
    
    // Turn left until right intersection sensor sees white
    slowdown_left = 7;
    slowdown_right = 0;

    if ( millis() - last_turn_start > 1300 ){
      state = FOLLOW_LINE;
    }
    //do_ir();
  }
  
  // Update the drive speeds
  drive(LEFT_ZERO+7-slowdown_left, RIGHT_ZERO+7-slowdown_right);
  //drive (170, 170);
  
  if (send_flag && ir_done == 1 && (millis() - last_send_time > SEND_TIMER)) {
    send_flag = 0;
    ir_done = 1;
    //i++;
    //wireless_send(&i, sizeof(int));
    //Serial.println(i);
    Serial.print("Sending pos: ");
    Serial.print(prev_pos.x);
    Serial.println(prev_pos.y);
    tile_transmit(prev_pos);
  }
  // Print out readings
  uint8_t walls = 0;

  /*if (poll_condition(D_WALL_FRONT, 10)) digitalWrite(FRONT_LED, HIGH);
  else digitalWrite(FRONT_LED, LOW);

  if (poll_condition(D_WALL_LEFT, 10)) digitalWrite(LEFT_LED, HIGH);
  else digitalWrite(LEFT_LED, LOW);

  if (poll_condition(D_WALL_RIGHT, 10)) digitalWrite(RIGHT_LED, HIGH);
  else digitalWrite(RIGHT_LED, LOW);*/
  
  /*Serial.print(poll_condition(D_WALL_FRONT, 10)? "F: Y " : "F; N ");
  Serial.print(poll_condition(D_WALL_RIGHT, 10)? "R: Y " : "R: N ");
  Serial.println(poll_condition(D_WALL_LEFT, 10)? "L: Y " : "L: N ");*/
  /*Serial.print("L: ");
  Serial.print(analogRead(P_INTERSECT_SENSOR_1));
  Serial.print("  R: ");
  Serial.print(analogRead(P_INTERSECT_SENSOR_2));
  Serial.print("  D: ");
  Serial.println(abs(line_left_value - line_right_value));*/
}

void drive(int left, int right){
  servo_left.write(left);
  servo_right.write(180-right);
}

