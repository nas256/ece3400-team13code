#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

extern RF24 radio;

void wireless_setup(uint8_t wr);

bool wireless_send(uint8_t * buf, uint8_t buf_size);

bool wireless_read(uint8_t * buf, uint8_t rd_length);
