// Line following code
// ECE 3400 - Group 13

// Low level: 528
// High Level: 977

#include <Servo.h>

#include "debounce.h"
#include "debounce.c" //whaaaaaat


// Pin Definitions
#define P_LINE_SENSOR_1        A1
#define P_LINE_SENSOR_2        A0
#define P_INTERSECT_SENSOR_1   A2
#define P_INTERSECT_SENSOR_2   A3
#define P_SERVO_LEFT           10
#define P_SERVO_RIGHT          11

// Light Level Parameters
#define WHITE_VALUE            700  // 538
#define BLACK_VALUE            976
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

// Turns
#define LEFT                   0
#define RIGHT                  1

char figure_8[] = { RIGHT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT };
char figure_8_index = 0;

unsigned long last_turn_start = 0;


Servo servo_left, servo_right;
int line_left_value = 0, line_right_value = 0;
char state = FOLLOW_LINE;

void setup() {
  Serial.begin(115200); // Begin UART Communication

  // Initialize servo variables
  servo_left.attach(P_SERVO_LEFT);
  servo_right.attach(P_SERVO_RIGHT);

  // Setup Line Sensor Inputs
  pinMode(INPUT, P_LINE_SENSOR_1);
  pinMode(INPUT, P_LINE_SENSOR_2);
  pinMode(INPUT, P_INTERSECT_SENSOR_1);
  pinMode(INPUT, P_INTERSECT_SENSOR_2);

  drive(LEFT_ZERO, RIGHT_ZERO);

  delay(2000);
}

int slowdown_left = 0, slowdown_right = 0;

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
     clear_condition(D_INTERSECT_2_W);
     
   if ( analogRead(P_INTERSECT_SENSOR_1) < WHITE_VALUE + 30 )
     signal_condition(D_INTERSECT_1_W);
   else
     clear_condition(D_INTERSECT_1_W);
     
   if ( analogRead(P_INTERSECT_SENSOR_2) < WHITE_VALUE + 30 )
     signal_condition(D_INTERSECT_2_W);
   else
     clear_condition(D_INTERSECT_2_W);
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
        last_turn_start = millis();
  
        if (figure_8[figure_8_index] == LEFT){
          //Serial.println("I reached! Turning left");
          //Serial.println(analogRead(P_INTERSECT_SENSOR_2));
          state = TURN_LEFT;
        }else if (figure_8[figure_8_index] == RIGHT) {
          //Serial.println("I reached! Turning right");
          state = TURN_RIGHT;
        }

        figure_8_index++;
        if (figure_8_index > 7)
          figure_8_index = 0;
        Serial.print("NEW INDEX: ");
        Serial.println((int)figure_8_index);
      }
    }
    
    
  } else if (state == TURN_RIGHT){
    // Turn right until right intersection sensor sees white
    slowdown_left = 0;
    slowdown_right = 7;

    if ( millis() - last_turn_start > 1100 )
      state = FOLLOW_LINE;
         
  } else if (state == TURN_LEFT){
    
    // Turn right until right intersection sensor sees white
    slowdown_left = 7;
    slowdown_right = 0;

    if ( millis() - last_turn_start > 1100 )
      state = FOLLOW_LINE;
  }
  
  // Update the drive speeds
  drive(LEFT_ZERO+7-slowdown_left, RIGHT_ZERO+7-slowdown_right);
  
  // Print out readings
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

