#include <timeout.h>
#include <RH_RF95.h>
#include "src/screen/gui.h"

// Pinout
#define RFM95_INT 3
#define RFM95_RST 4
#define RFM95_CS 8

#define BATT_MON A7 // LiPo

#define HEARTBEAT_LED_DELAY_MS 500
#define BUTTON_DEBOUNCE_MS 250
#define DRIVE_RC_REFRESH_MS 40 // 25Hz
#define SCREEN_UPDATE_RATE_MS 20 // 50Hz

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0


// Singleton instance of the radio driver
RH_RF95 radio(RFM95_CS, RFM95_INT);
char radio_tx_buffer[16] = "Hello World    ";

Timeout led_timer,gui_timer;

////////////////
//            //
// Setup      //
//            //
////////////////
void setup() {
  pinMode(PIN_LED,OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  pinMode(RFM95_CS, OUTPUT);
  digitalWrite(RFM95_CS, HIGH);
   
  Serial.begin(57600);
  while(!Serial);

  gui_init();  

  // reset radio
  digitalWrite(RFM95_RST, HIGH); delay(10);
  digitalWrite(RFM95_RST, LOW);  delay(10);
  digitalWrite(RFM95_RST, HIGH); delay(10);  

  uint8_t device_version = radio.getDeviceVersion();
  Serial.print(F("RFM95 Version ID: "));
  Serial.print(device_version);
  Serial.print(F("\n"));

  // Initialize radio
  if (!radio.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Set 
  if (!radio.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //radio.setTxPower(23, false);
  
  radio.printRegisters();

  led_timer.start(HEARTBEAT_LED_DELAY_MS);
  gui_timer.start(SCREEN_UPDATE_RATE_MS);
}

////////////////
//            //
// Main Loop  //
//            //
////////////////
void loop() {
  static bool led_state = false;

  if(led_timer.periodic()){
    led_state = !led_state;
    digitalWrite(PIN_LED,led_state);
  }

  if(gui_timer.periodic()){
    gui_update();
  }

  //radio.send((uint8_t *)radio_tx_buffer, 16);
}
