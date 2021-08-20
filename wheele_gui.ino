#include <GUIslice.h>
#include <RH_RF95.h>
#include <timeout.h>
#include "src/radio/radio.h"
#include "src/gui/gui.h"

#define BATT_MON A7 // LiPo

#define HEARTBEAT_LED_DELAY_MS 500
#define BUTTON_DEBOUNCE_MS 250
#define DRIVE_RC_REFRESH_MS 40 // 25Hz
#define SCREEN_UPDATE_RATE_MS 20 // 50Hz

RH_RF95 radio(RFM95_CS, RFM95_INT);
gslc_tsGui  gui;
Timeout led_timer;
Timeout gui_timer;

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

  radio_init(&radio);
  gui_init(&gui);  

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
}
