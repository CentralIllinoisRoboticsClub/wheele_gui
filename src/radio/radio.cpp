#include "radio.h"

static RH_RF95 * s_pRadio;

void radio_init(RH_RF95 * pRadio)
{
   // reset radio
   digitalWrite(RFM95_RST, HIGH); delay(10);
   digitalWrite(RFM95_RST, LOW);  delay(10);
   digitalWrite(RFM95_RST, HIGH); delay(10);
   
   uint8_t device_version = pRadio->getDeviceVersion();
   Serial.print(F("RFM95 Version ID: "));
   Serial.print(device_version);
   Serial.print(F("\n"));

   // Initialize radio
   if (!pRadio->init()) {
     Serial.println("LoRa radio init failed");
     Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
     while (1);
   }
   Serial.println("LoRa radio init OK!");

   // Set
   if (!pRadio->setFrequency(RF95_FREQ)) {
     Serial.println("setFrequency failed");
     while (1);
   }
   Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

   // The default transmitter power is 13dBm, using PA_BOOST.
   // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
   // you can set transmitter powers from 5 to 23 dBm:
   //pRadio->setTxPower(23, false);

   pRadio->printRegisters();

   s_pRadio = pRadio;
}

void radio_tx(char tx_buffer[],uint8_t size)
{
   s_pRadio->send((uint8_t *)tx_buffer, size);
}
