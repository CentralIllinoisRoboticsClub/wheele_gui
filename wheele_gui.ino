#include <GUIslice.h>
#include <RH_RF95.h>
#include <SerialTransfer.h>
#include <timeout.h>
#include <Adafruit_GPS.h>
#include <Adafruit_SleepyDog.h>
#include "src/radio/radio.h"
#include "src/gui/gui.h"
#include "src/packet/packet.h"
#include "git-version.h"

#define BATT_MON A7 // LiPo
#define SCREEN_LITE_PIN 12
#define DEFAULT_BRIGHTNESS 200
#define SCREEN_OFF 0 

#define HEARTBEAT_LED_DELAY_MS 500
#define BUTTON_DEBOUNCE_MS 250
#define DRIVE_RC_REFRESH_MS 40 // 25Hz
#define SCREEN_UPDATE_RATE_MS 20 // 50Hz
#define VBATT_SAMPLE_RATE_MS 5000
#define ROS_HEARTBEAT_MS 5000
#define GPS_UPDATE_RATE_MS 2000
#define STATS_UPDATE_RATE_MS 5000
#define INACTIVITY_TIMEOUT_MS 60000
#define MAX_STR 32

//#define SERIAL_DEBUG_ENABLE

RH_RF95 radio(RFM95_CS, RFM95_INT);
gslc_tsGui  gui;
Timeout led_timer;
Timeout gui_timer;
Timeout battery_timer;
Timeout ros_heartbeat;
Timeout gps_update;
Timeout stats;
SerialTransfer serial_transfer;
Adafruit_GPS GPS(&Serial1);

char s_buf[MAX_STR] = {0};
unsigned long min_time_us = ULONG_MAX;
unsigned long max_time_us = 0;
float avg_time_us = 0.0;
unsigned long loop_count = 0;
unsigned long last_touch_time = 0;
bool screen_blank_active = false;
bool radio_on = true;
bool gps_on = true;

// This callback function is triggered any time
// that a touch event occurs while we have requested
// a touch trigger callback.
bool CbTouchTrig(void* pvGui)
{
  if (screen_blank_active) {
    // Enable the screen
    analogWrite(SCREEN_LITE_PIN,DEFAULT_BRIGHTNESS);
    screen_blank_active = false;

    // Wake-up radio
    radio.setModeIdle();
    radio_on = true;

    // Wake-up GPS
    GPS.wakeup();
    gps_on = true;
  }
  
  // Reset timer
  last_touch_time = millis();
  gslc_SetTouchTrigFunc(&gui,&CbTouchTrig);

  return true;
}

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
  analogWrite(SCREEN_LITE_PIN,DEFAULT_BRIGHTNESS);

  Serial.begin(57600);
  //while(!Serial);
  for(int i=0;i<1000;i++);

  Serial.println("WHEELE GUI Version: " GIT_VERSION);

  radio_init(&radio);
  gui_init(&gui);
  
  serial_transfer.begin(Serial);
    
  GPS.begin(9600);  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); // turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA); // Request updates on antenna status
  GPS.sendCommand(PMTK_Q_RELEASE); // Request GPS firmware version 

  led_timer.start(HEARTBEAT_LED_DELAY_MS);
  gui_timer.start(SCREEN_UPDATE_RATE_MS);
  battery_timer.start(VBATT_SAMPLE_RATE_MS);
  ros_heartbeat.start(ROS_HEARTBEAT_MS);
  gps_update.start(GPS_UPDATE_RATE_MS);
  stats.start(STATS_UPDATE_RATE_MS);
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
  unsigned long start_time,finish_time,loop_time;

  start_time = micros();
  
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    GPS.parse(GPS.lastNMEA());
  }
  
  if(led_timer.periodic()){
    led_state = !led_state;
    digitalWrite(PIN_LED,led_state);
  }

  if(gps_update.periodic()){
    if(gps_on == true){
      pGuiElement = gslc_PageFindElemById(&gui,E_PG_CTRL,E_ELEM_CTRL_ID_GPS_BOX);
      gslc_tsColor box_color = GSLC_COL_RED_DK1;
      if(GPS.fix){
        gslc_tsElemRef* pPositionTxt;
        box_color = GSLC_COL_GREEN_DK1;
    
        pPositionTxt = gslc_PageFindElemById(&gui,E_PG_WAYPOINTS,E_ELEM_WYPT_ID_GPS_LAT);
        snprintf(s_buf,MAX_STR,"LAT:%f%c",GPS.latitudeDegrees,GPS.lat);
        gslc_ElemSetTxtStr(&gui,pPositionTxt,(const char*)s_buf);
        gslc_ElemSetRedraw(&gui,pPositionTxt,GSLC_REDRAW_FULL);
  
        pPositionTxt = gslc_PageFindElemById(&gui,E_PG_WAYPOINTS,E_ELEM_WYPT_ID_GPS_LON);
        snprintf(s_buf,MAX_STR,"LON:%f%c",GPS.longitudeDegrees,GPS.lon);
        gslc_ElemSetTxtStr(&gui,pPositionTxt,(const char*)s_buf);
        gslc_ElemSetRedraw(&gui,pPositionTxt,GSLC_REDRAW_FULL);
      }
      gslc_ElemSetCol(&gui,pGuiElement,GSLC_COL_BLACK,box_color,GSLC_COL_BLUE);
      gslc_ElemSetRedraw(&gui,pGuiElement,GSLC_REDRAW_FULL);    
    }  
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

  if(radio_on == true){
    if(radio.available()){
      radio_rx();
    }
  }
   
  if(gui_timer.periodic()){
    gslc_Update(&gui);
  }

  if(!screen_blank_active){
    if(millis() - last_touch_time > INACTIVITY_TIMEOUT_MS){
      // Disable screen
      screen_blank_active = true;
      analogWrite(SCREEN_LITE_PIN,SCREEN_OFF);
      gslc_SetTouchTrigFunc(&gui,&CbTouchTrig);
    }
  }
  else{
    if(millis() - last_touch_time > (2*INACTIVITY_TIMEOUT_MS)){
      if(radio_on == true){
        // Shutdown radio
        if(radio.sleep()){
          Serial.println("Power down radio");
          radio_on = false;
        }
      }
      if(gps_on == true){
        GPS.standby();
        gps_on = false;
      }

      digitalWrite(PIN_LED,0);
      while(screen_blank_active){
        // wake up once per second to check for touchscreen press
        Watchdog.sleep(1000);
        gslc_Update(&gui);
      }
    }
  }

  finish_time = micros();
  loop_count++;
  loop_time = finish_time - start_time;
  if(loop_time > max_time_us){max_time_us = loop_time;}
  if(loop_time < min_time_us){min_time_us = loop_time;}
  avg_time_us = (float)(avg_time_us + (float)loop_time)/2;

  if(stats.periodic()){
    #ifdef SERIAL_DEBUG_ENABLE
    Serial.print("loop time (min): "); Serial.print(min_time_us); Serial.println(" us");
    Serial.print("loop time (max): "); Serial.print(max_time_us); Serial.println(" us");
    Serial.print("loop time (avg): "); Serial.print(avg_time_us); Serial.println(" us");
    #endif
    min_time_us = ULONG_MAX; max_time_us = 0;
  }
}
