#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "wireless1.h"

#define X_SIZE 4
#define Y_SIZE 5

//same x and y bits
//2nd: current location
//3r: already been there
struct grid{
     char traversed;
     char current_location;
};
typedef struct grid grid;

unsigned char x_coord = 0;
unsigned char y_coord = 0;
int turn = 0;
unsigned int maze [4][5];

uint8_t done = 0;

void setup(void)
{

  Serial.begin(57600);

  
  printf_begin();
  pinMode(7,OUTPUT);
  SPI.begin();
  wireless_setup(0);

  // Initialize maze
  for (int i = 0; i < X_SIZE; i++) {
    for (int j = 0; j < Y_SIZE; j++) {
      maze[i][j] = 0;
      maze[i][j] |= ((i&3) << 14) | ((j&7) << 11);
      Serial.println(maze[i][j]);
    }
  }

  radio.stopListening();

  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0)); //10MHz

  for (uint16_t x = 0; x < X_SIZE; x++){
    for (uint16_t y = 0; y < Y_SIZE; y++){
      digitalWrite(7, LOW);
      delay(1);
      SPI.transfer16(maze[x][y]);
      digitalWrite(7, HIGH);
    }
  }
  // need to rewrite 0 for some reason
  digitalWrite(7, LOW);
  delay(1);
  SPI.transfer16(0);
  digitalWrite(7, HIGH);

  SPI.endTransaction();


  radio.startListening();
}


void loop(void)
{
  // if there is data ready
  //if ( radio.available() )
  //{
    Serial.println("Available!");
    // Dump the payloads until we've gotten everything
  uint16_t got_data;
  bool timeout = true;
  while (timeout)
  {
    // Fetch the payload, and see if this was the last one.
    timeout = wireless_read( &got_data, sizeof(uint16_t) ); // 200ms timeout
    //done = radio.read( &got_data, sizeof(got_data) );
  
    // Spew it
    // Print the received data as a decimal
    //if (timeout) continue;
 
  
    // Delay just a little bit to let the other unit
    // make the transition to receiver
    // delay(20);
  
  } 

     printf("Got payload %#04x (%u)", got_data, got_data);
    printf("\n");

    if (got_data & 0x1) done = 1;

    // First, stop listening so we can talk over SPI
    radio.stopListening();
    
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); //10MHz

    unsigned char got_x = got_data >> 14;
    unsigned char got_y = (got_data >> 11) & 7;
    maze[got_x][got_y] = got_data;
    for (uint16_t x = 0; x < X_SIZE; x++){
      for (uint16_t y = 0; y < Y_SIZE; y++){
        if (maze[x][y] == 0) continue;
        printf("x: %d, y: %d, %#04x ", x, y, maze[x][y]);
        printf("\n");
        if ((x != got_x) || (y != got_y))  maze[x][y] = maze[x][y] & 0xfffd;
        else                               maze[x][y] = maze[x][y]  | 1 << 1;
        digitalWrite(7, LOW);
        delay(1);
        SPI.transfer16(maze[x][y] | done);
        //SPI.transfer16(0x46);
        digitalWrite(7, HIGH);
      }
    }

    SPI.endTransaction();
    // Send the final one back.
    //radio.write( &got_data, sizeof(got_data) );
    //printf("Sent response.\n\r");

    // Now, resume listening so we catch the next packets.
    radio.startListening();
  //}
}
