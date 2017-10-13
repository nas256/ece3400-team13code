
#include <SPI.h>
//pin 13 is SCK
//pin 11 is MOSI
//pin 10 is ~SS

#define X_SIZE 5
#define Y_SIZE 4

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT); // Slave select pin
  SPI.begin();
}

void loop() {
  delay(250); // Its supposed to be a dance party, not an epilepsy party

  // Doesn't work well at < 10MHz
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); //10MHz

  for (uint16_t x = 0; x < X_SIZE; x++){
    for (uint16_t y = 0; y < Y_SIZE; y++){
      uint16_t result = 0;
      result |= (x << 8) | (y << 12); // Write coords
      result |= random(0, 255) & 0x00FF; // Write random color

      // Write result out
      digitalWrite(10, LOW);
      SPI.transfer16(result);
      digitalWrite(10, HIGH);
    }
  }
  
  SPI.endTransaction();
}





