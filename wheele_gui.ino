#include "screen.h"
#include <RH_RF95.h>

// Pinout
#define TFT_DC 0 // data/command select
#define TFT_CS 1 // SPI chip select
#define RFM95_INT 3
#define RFM95_RST 4
#define RFM95_CS 8

#define TS_YM 14   // touchscreen y-; can be any digital pin
#define TS_XP 15  // touchscreen x+; can be any digital pin

#define TS_XM A4 // touchscreen x-; must be an analog pin, use "An" notation!
#define TS_YP A5 // touchscreen y+; must be an analog pin, use "An" notation!
#define BATT_MON A7 // LiPo

// Touch detection constants
#define MINPRESSURE 100
#define MAXPRESSURE 1000

// Calibration data for the raw touch data to the screen coordinates
//#define PRINT_RAW_TOUCHPOINT
#define TS_MINX 306
#define TS_MAXX 800
#define TS_MINY 220
#define TS_MAXY 860

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
#define TS_RES_OHMS 300

#define HEARTBEAT_LED_DELAY_MS 500
#define BUTTON_DEBOUNCE_MS 250
#define DRIVE_RC_REFRESH_MS 40 // 25Hz

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Touchscreen objects
Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);
TouchScreen grid = TouchScreen(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES_OHMS);

// Singleton instance of the radio driver
RH_RF95 radio(RFM95_CS, RFM95_INT);
char radio_tx_buffer[16] = "Hello World    ";

int current_screen, new_screen;

bool touch_detected(TSPoint& point){
  TSPoint p = grid.getPoint();
  int16_t x = p.x;
  int16_t y = p.y;

#ifdef PRINT_RAW_TOUCHPOINT
// for calibration...
  Serial.print("Raw touch...");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print("\r");
#endif

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     // no touch
     return false;
  }
  else{ // touch
    // Scale from ~0->1000 to tft.width using the calibration #'s
    // NOTE: rotated 90 degrees to match landscape orientation of screen (x = TS_MAXY - y, y = x)
    p.x = map(TS_MAXY - y, 0, TS_MAXY - TS_MINY, 0, screen.width());
    p.y = map(x, TS_MINX, TS_MAXX, 0, screen.height());
    point = p;
    return true;
  }
}

bool touch_detected_test(TSPoint& point){
  int c;
  
  if(Serial.available() > 0)
  {
    c = Serial.read();
    switch(c){
      default:
        point.x = 30;
        point.y = 130;
        break;
    }
    return true;
  }
  else{
    return false;
  }
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
   
  Serial.begin(57600);
  while(!Serial);

  Serial.print(F("Loading CIRC Wheele Display..."));
  screen.begin();
  screen.setRotation(3);
  draw_home_screen(screen);
  new_screen = current_screen = HOME_SCREEN;
  Serial.print(F("finished.\n"));

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
}

////////////////
//            //
// Main Loop  //
//            //
////////////////
void loop() {
  int c;
  TSPoint point;
  static unsigned long button_press_time = 0;
  static unsigned long heartbeat_led_time = 0;
  static int pin_state = 0;
  unsigned long current_time;

  current_time = millis();
  if((current_time - heartbeat_led_time) > HEARTBEAT_LED_DELAY_MS){
    heartbeat_led_time = current_time;
    if(pin_state == 0){
      pin_state = 1;
      digitalWrite(PIN_LED, HIGH);
    }
    else{
      pin_state = 0;
      digitalWrite(PIN_LED, LOW);
    }
  }

  switch(current_screen){
    case HOME_SCREEN:
      if(touch_detected(point)){
        button_press_time = current_time;
        new_screen = touch_home_screen(screen,point);
      }
      break;

    case SHUTDOWN_SCREEN:
      update_shutdown_screen(screen);
      if(touch_detected(point)){
        new_screen = touch_shutdown_screen(screen,point);
      }
      break;

    case WAYPOINTS_SCREEN:
      update_waypoints_screen(screen);
      if((current_time - button_press_time) > BUTTON_DEBOUNCE_MS){
        if(touch_detected(point)){
          button_press_time = current_time;
          new_screen = touch_waypoints_screen(screen,point);
        }
      }
      break;

    case DRIVE_RC_SCREEN:
      update_drive_rc_screen(screen);
      if((current_time - button_press_time) > DRIVE_RC_REFRESH_MS){
        if(touch_detected(point)){
          button_press_time = current_time;
          new_screen = touch_drive_rc_screen(screen,point);
          radio.send((uint8_t *)radio_tx_buffer, 16);
        }   
      }
      break;
      
    default:
      break;
  }

  if(new_screen != current_screen){
    switch(new_screen){
      case HOME_SCREEN:
        draw_home_screen(screen);
        break;

      case SHUTDOWN_SCREEN:
        draw_shutdown_screen(screen);
        break;

      case WAYPOINTS_SCREEN:
        draw_waypoints_screen(screen);
        break;

      case DRIVE_RC_SCREEN:
        draw_drive_rc_screen(screen);
        break;

      default:
        break;
    }
    current_screen = new_screen;
  }
}
