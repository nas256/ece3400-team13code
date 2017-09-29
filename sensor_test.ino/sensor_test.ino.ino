// Line following code
// ECE 3400 - Group 13

// Low level: 528
// High Level: 977

#include <Servo.h>

// Pin Definitions
#define P_LINE_SENSOR_1        A1 // Left
#define P_LINE_SENSOR_2        A0 // Right
#define P_INTERSECT_SENSOR_1   A2 // Left
#define P_INTERSECT_SENSOR_2   A3 // Right
#define P_WALL_FRONT           A4
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


void loop() {  
  // Update the drive speeds
  drive(LEFT_ZERO, RIGHT_ZERO);
  
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

