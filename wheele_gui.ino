#include <GUIslice.h>
#include <RH_RF95.h>
#include <SerialTransfer.h>
#include <timeout.h>
#include "src/radio/radio.h"
#include "src/gui/gui.h"
#include "src/packet/packet.h"
#include "git-version.h"

#define BATT_MON A7 // LiPo

#define HEARTBEAT_LED_DELAY_MS 500
#define BUTTON_DEBOUNCE_MS 250
#define DRIVE_RC_REFRESH_MS 40 // 25Hz
#define SCREEN_UPDATE_RATE_MS 20 // 50Hz
#define VBATT_SAMPLE_RATE_MS 5000
#define ROS_HEARTBEAT_MS 5000
#define MAX_STR 32

RH_RF95 radio(RFM95_CS, RFM95_INT);
gslc_tsGui  gui;
Timeout led_timer;
Timeout gui_timer;
Timeout battery_timer;
Timeout ros_heartbeat;
SerialTransfer serial_transfer;

static char s_buf[MAX_STR] = {0};

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
  //while(!Serial);
  for(int i=0;i<1000;i++);

  Serial.println("WHEELE GUI Version: " GIT_VERSION);

  radio_init(&radio);
  gui_init(&gui);
  
  serial_transfer.begin(Serial);
  
  led_timer.start(HEARTBEAT_LED_DELAY_MS);
  gui_timer.start(SCREEN_UPDATE_RATE_MS);
  battery_timer.start(VBATT_SAMPLE_RATE_MS);
  ros_heartbeat.start(ROS_HEARTBEAT_MS);
}

////////////////
//            //
// Main Loop  //
//            //
////////////////
void loop() {
  static bool led_state = false;
  static bool ros_alive = false;
  gslc_tsElemRef* pGuiElement;

  if(led_timer.periodic()){
    led_state = !led_state;
    digitalWrite(PIN_LED,led_state);
  }

  if(ros_heartbeat.periodic()){
    pGuiElement = gslc_PageFindElemById(&gui,E_PG_CTRL,E_ELEM_CTRL_ID_ROS_BOX);
    gslc_tsColor box_color = GSLC_COL_RED_DK1;
    if(ros_alive){
      ros_alive = false; // must refresh within timer period
      box_color = GSLC_COL_GREEN_DK1;
    }
    gslc_ElemSetCol(&gui,pGuiElement,GSLC_COL_BLACK,box_color,GSLC_COL_BLUE);
    gslc_ElemSetRedraw(&gui,pGuiElement,GSLC_REDRAW_FULL);
  }

  if(battery_timer.periodic()){
    uint32_t vbatt_millivolts;
    uint16_t adc_counts = analogRead(BATT_MON);

    // 2x voltage divider, 3.3V reference, 10-bit ADC
    vbatt_millivolts = (2*3300*adc_counts) >> 10; 

    // update screen
    pGuiElement = gslc_PageFindElemById(&gui,E_PG_INFO,E_ELEM_INFO_ID_VBATT);
    snprintf(s_buf,MAX_STR,"GUI BATT:%dmV\0",vbatt_millivolts);
    gslc_ElemSetTxtStr(&gui,pGuiElement,(const char*)s_buf);
    gslc_ElemSetRedraw(&gui,pGuiElement,GSLC_REDRAW_FULL);
  }

  if(serial_transfer.available()){
    if(serial_transfer.currentPacketID() == PACKET_ID_ROS_ALIVE){
      ros_alive = true;
      serial_transfer.packet.reset();
    }
    else{
      packet_handler(serial_transfer);
    }
  }

  if(radio.available()){
    radio_rx();
  }
   
  if(gui_timer.periodic()){
    gslc_Update(&gui);
  }
}
