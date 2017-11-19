#include "wireless1.h"

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000002cLL, 0x000000002dLL };

RF24 radio(9, 10);

void wireless_setup(uint8_t wr){
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_HIGH);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  // radio.setPayloadSize(8);

  if ( wr )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  radio.startListening();
}

bool wireless_send(void* buf, uint16_t buf_size){

  radio.stopListening();
  
  uint8_t sent = radio.write(buf, buf_size);

  while (!sent) sent = radio.write(buf, buf_size);

  return sent;
}

bool wireless_read(void * buf, uint16_t rd_length){
  radio.startListening();
  
  uint8_t timeout = false;
  unsigned long started_waiting_at = millis();
  while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

  if (!timeout){
    radio.read(buf, rd_length);
  }

  return timeout;
}
