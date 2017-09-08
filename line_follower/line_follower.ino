// Line following code
// ECE 3400 - Group 13

// Low level: 528
// High Level: 977

#include <Servo.h>

// Pin Definitions
#define P_LINE_SENSOR_1        A1
#define P_LINE_SENSOR_2        A0
#define P_INTERSECT_SENSOR_1   A2
#define P_INTERSECT_SENSOR_2   A3
#define P_SERVO_LEFT           10
#define P_SERVO_RIGHT          11

// Light Level Parameters
#define WHITE_VALUE            538
#define BLACK_VALUE            976
#define LIGHT_TOLERANCE        5

// Define servo tuning parameters
#define LEFT_MAX               180
#define RIGHT_MAX              180
#define LEFT_ZERO              94
#define RIGHT_ZERO             86

Servo servo_left, servo_right;
int line_left_value = 0, line_right_value = 0;

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

  drive(LEFT_ZERO, RIGHT_ZERO);

  delay(2000);
}

int slowdown_left = 0, slowdown_right = 0;

void loop() {  
  // Update readings from line sensors
  line_left_value = analogRead(P_LINE_SENSOR_1);  // 0-1023
  line_right_value = analogRead(P_LINE_SENSOR_2); // 0-1023

  // Line following adjustments
  if ( abs(line_left_value - line_right_value) < LIGHT_TOLERANCE ){
    // drive straight
    slowdown_left = slowdown_right = 0;
  }else if (line_left_value > line_right_value){
    // turn left
    slowdown_left = map(line_left_value-line_right_value, 0, BLACK_VALUE-WHITE_VALUE, 0, 150);
    slowdown_right = 0;
  }else if (line_left_value < line_right_value){
    // turn right
    slowdown_left = 0;
    slowdown_right = map(line_right_value-line_left_value, 0, BLACK_VALUE-WHITE_VALUE, 0, 150);
  }
  
  // Update the drive speeds
  drive(LEFT_ZERO+40-slowdown_left, RIGHT_ZERO+40-slowdown_right);
  
  // Print out readings
  /*Serial.print("L: ");
  Serial.print(line_left_value);
  Serial.print("  R: ");
  Serial.print(line_right_value);
  Serial.print("  D: ");
  Serial.println(abs(line_left_value - line_right_value));*/
}

void drive(int left, int right){
  servo_left.write(left);
  servo_right.write(180-right);
}

