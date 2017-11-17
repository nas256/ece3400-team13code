#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "wireless1.h"

#define X_SIZE 5
#define Y_SIZE 4

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
unsigned int maze [5][4];

void setup(void)
{

  // Initialize maze
  
  int i = 0;
  int j = 0;
  for (; i < 5; i++) {
    for (; j < 4; j++) {
      maze[i][j] = i << 13 | j << 11;
    }
  }
  
  Serial.begin(57600);
  printf_begin();
  pinMode(7,OUTPUT);
  SPI.begin();
}


void loop(void)
{
  // if there is data ready
  if ( radio.available() )
  {
    // Dump the payloads until we've gotten everything
  unsigned int got_data;
  bool done = false;
  while (!done)
  {
    // Fetch the payload, and see if this was the last one.
    done = wireless_read( &got_data, sizeof(got_data) ); // 200ms timeout
    //done = radio.read( &got_data, sizeof(got_data) );
  
    // Spew it
    // Print the received data as a decimal
    printf("Got payload %u...", got_data);
  
    // Delay just a little bit to let the other unit
    // make the transition to receiver
    delay(20);
  
  } 

    // First, stop listening so we can talk over SPI
    radio.stopListening();
    
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); //10MHz

    unsigned char got_x = got_data >> 13;
    unsigned char got_y = (got_data >> 11) & 3;
    maze[got_x][got_y] = got_data;
    for (uint16_t x = 0; x < X_SIZE; x++){
      for (uint16_t y = 0; y < Y_SIZE; y++){
        if ((x != got_x) || (y != got_y)) maze[x][y] = maze[x][y] & 0xfffd;
        digitalWrite(7, LOW);
        SPI.transfer16(maze[x][y]);
        digitalWrite(7, HIGH);
      }
    }

    SPI.endTransaction();
    // Send the final one back.
    //radio.write( &got_data, sizeof(got_data) );
    //printf("Sent response.\n\r");

    // Now, resume listening so we catch the next packets.
    radio.startListening();
  }
}
