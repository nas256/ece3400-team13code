#include <SPI.h>
//pin 13 is SCK
//pin 11 is MOSI
//pin 10 is ~SS

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

grid array_grid[5][4];

int current_x = 0;
int current_y = 0;
int turn = 0;

int done = 0;

void spi_communicate(){
     //current best at 1 MHz
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); //10MHz

  for (uint16_t x = 0; x < X_SIZE; x++){
    for (uint16_t y = 0; y < Y_SIZE; y++){
      uint16_t result = 0;
      result |= (((x& 0x7) << 13)) | ((y& 0x3) << 11); // Write coords
      result |= array_grid[x][y].traversed << 2;
      result |= array_grid[x][y].current_location << 1;
      result |= 1 << 6; // north wall
      result |= 1 << 5; // north wall
      result |= 1 << 4; // north wall
      result |= 1 << 3; // north wall

      result |= 3 << 9; // treasure
      
      result |= done; // sound
       
      // Write result out
      digitalWrite(7, LOW);
      SPI.transfer16(result);
      digitalWrite(7, HIGH);
    }
  }
  
  SPI.endTransaction();
  
}
void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT); // Slave select pin
  SPI.begin();
}

void loop() {
  delay(250); // Its supposed to be a dance party, not an epilepsy party

  spi_communicate();

  array_grid[current_x][current_y].current_location = 0;
  array_grid[current_x][current_y].traversed = 1;

  if (done == 1) return;
  
  if (turn == 0) current_x++;
  if (turn == 1) current_x--;
  if (current_x > 4){ 
    turn = 1;
    current_y++;
    current_x--;
  }
  if (current_x < 0) {
    turn = 0;
    current_y++;
    current_x++;
  }

  if (current_y > 3){
    current_y = 0;
    done = 1;
  }
  if (done == 1) return;
  array_grid[current_x][current_y].current_location = 1;
  
  
  }
 


