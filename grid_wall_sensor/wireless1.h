#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

extern RF24 radio;

void wireless_setup(uint8_t wr);

bool wireless_send(uint16_t * buf, uint16_t buf_size);

bool wireless_read(uint16_t * buf, uint16_t rd_length);
