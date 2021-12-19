#ifndef _RADIO_H_
#define _RADIO_H_

#include <RH_RF95.h>

// Pinout
#define RFM95_INT 3
#define RFM95_RST 4
#define RFM95_CS 8

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

void radio_init(RH_RF95 * pRadio);
void radio_tx(char msg[],uint8_t size);
void radio_rx(void);
float radio_get_chan_center_freq(int chan);
void radio_set_frequency(float f);
uint8_t radio_get_version(void);

#endif
