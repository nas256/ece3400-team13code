// Line following code
// ECE 3400 - Group 13

// Low level: 528
// High Level: 977

#include <Servo.h>

#include "debounce.h"
#include "debounce.c" // i hate you arduino ide trash

#include "amux.h"

// Pin Definitions
#define P_LINE_SENSOR_1        A1 // Left
#define P_LINE_SENSOR_2        A0 // Right
#define P_INTERSECT_SENSOR_1   A2 // Left
#define P_INTERSECT_SENSOR_2   A3 // Right
#define P_AMUX                 A4
#define P_WALL_FRONT           A5

#define P_SERVO_LEFT           5
#define P_SERVO_RIGHT          6

// Light Level Parameters
#define WHITE_VALUE            538  // 700
#define BLACK_VALUE            850
#define LIGHT_TOLERANCE        5

// Define servo tuning parameters
#define LEFT_MAX               180
#define RIGHT_MAX              180
#define LEFT_ZERO              94
#define RIGHT_ZERO             86

// Define operating states
#define FOLLOW_LINE            0
#define TURN_LEFT              1
#define TURN_RIGHT             2
#define TURN_180               3

// Turns
#define LEFT                   0
#define RIGHT                  1

#define WALL_THRESHOLD_SIDE    235
#define WALL_THRESHOLD_FRONT   190 //170

#define LEFT_LED 1
#define RIGHT_LED 0
#define FRONT_LED 7

#define FRONT_WALL_POLL_THRESHOLD 20
#define SIDE_WALL_POLL_THRESHOLD  20

unsigned long last_turn_start = 0;

Servo servo_left, servo_right;
int line_left_value = 0, line_right_value = 0;
char state = FOLLOW_LINE;

void setup() {
  //Serial.begin(115200); // Begin UART Communication

  // Initialize servo variables
  servo_left.attach(P_SERVO_LEFT);
  servo_right.attach(P_SERVO_RIGHT);

  // Setup Line Sensor Inputs
  pinMode(INPUT, P_LINE_SENSOR_1);
  pinMode(INPUT, P_LINE_SENSOR_2);
  pinMode(INPUT, P_INTERSECT_SENSOR_1);
  pinMode(INPUT, P_INTERSECT_SENSOR_2);
  pinMode(OUTPUT, LEFT_LED);
  pinMode(OUTPUT, RIGHT_LED);
  pinMode(OUTPUT, FRONT_LED);
  digitalWrite(LEFT_LED,LOW);
  digitalWrite(RIGHT_LED,LOW);
  digitalWrite(FRONT_LED,LOW);


  // Initialize the analog mux
  amux_init();

  drive(LEFT_ZERO, RIGHT_ZERO);

  delay(2000);
}

int slowdown_left = 0, slowdown_right = 0;

void sense_walls(){
  amux_select(WALL_SENSOR_RIGHT);
  delayMicroseconds(10);
  if ( analogRead(P_AMUX) > WALL_THRESHOLD_SIDE)
     signal_condition(D_WALL_RIGHT);
  else
     clear_condition(D_WALL_RIGHT);

  amux_select(WALL_SENSOR_LEFT);
  delayMicroseconds(10);
  if ( analogRead(P_AMUX) > WALL_THRESHOLD_SIDE)
     signal_condition(D_WALL_LEFT);
  else
     clear_condition(D_WALL_LEFT);
  Serial.println(analogRead(P_AMUX));

  if ( analogRead(P_WALL_FRONT) > WALL_THRESHOLD_FRONT)
     signal_condition(D_WALL_FRONT);
  else
     clear_condition(D_WALL_FRONT); 
}

void loop() { 
  
  // Update readings from line sensors
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

      if ( millis() - last_turn_start > 2000 ){
         
        if ( poll_condition(D_WALL_FRONT, FRONT_WALL_POLL_THRESHOLD)){
            last_turn_start = millis();
            if (poll_condition(D_WALL_RIGHT, SIDE_WALL_POLL_THRESHOLD)){ 
              state = TURN_LEFT; 
              if (poll_condition(D_WALL_LEFT, SIDE_WALL_POLL_THRESHOLD)) state = TURN_180;
            }
            else state = TURN_RIGHT;
        }
      }
    }
  } else if (state == TURN_180){
    // Turn right until right intersection sensor sees white
    slowdown_left = 0;
    slowdown_right = 14;

    if ( millis() - last_turn_start > 1500 )
      state = FOLLOW_LINE;
         
  
  } else if (state == TURN_RIGHT){
    // Turn right until right intersection sensor sees white
    slowdown_left = 0;
    slowdown_right = 7;

    if ( millis() - last_turn_start > 1450 )
      state = FOLLOW_LINE;
         
  } else if (state == TURN_LEFT){
    
    // Turn left until right intersection sensor sees white
    slowdown_left = 7;
    slowdown_right = 0;

    if ( millis() - last_turn_start > 1150 )
      state = FOLLOW_LINE;
  }
  
  // Update the drive speeds
  drive(LEFT_ZERO+7-slowdown_left, RIGHT_ZERO+7-slowdown_right);
  
  // Print out readings
  uint8_t walls = 0;

  if (poll_condition(D_WALL_FRONT, 10)) digitalWrite(FRONT_LED, HIGH);
  else digitalWrite(FRONT_LED, LOW);

  if (poll_condition(D_WALL_LEFT, 10)) digitalWrite(LEFT_LED, HIGH);
  else digitalWrite(LEFT_LED, LOW);

  if (poll_condition(D_WALL_RIGHT, 10)) digitalWrite(RIGHT_LED, HIGH);
  else digitalWrite(RIGHT_LED, LOW);
  
  //Serial.print(poll_condition(D_WALL_FRONT, 10)? "Y " : "N ");
  //Serial.print(state);
  //Serial.print(poll_condition(D_WALL_RIGHT, 10)? "Y " : "N ");
  //Serial.print(poll_condition(D_WALL_LEFT, 10)? "Y " : "N ");
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

