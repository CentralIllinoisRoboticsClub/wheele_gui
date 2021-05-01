#include "screen.h"

// Pinout
#define TFT_DC 49 // data/command select
#define TFT_CS 53 // SPI chip select
#define TS_XP 48  // touchscreen x+; can be any digital pin
#define TS_XM A14 // touchscreen x-; must be an analog pin, use "An" notation!
#define TS_YP A15 // touchscreen y+; must be an analog pin, use "An" notation!
#define TS_YM 46   // touchscreen y-; can be any digital pin

// Touch detection constants
#define MINPRESSURE 100
#define MAXPRESSURE 1000

// Calibration data for the raw touch data to the screen coordinates
//#define PRINT_RAW_TOUCHPOINT
#define TS_MINX 105
#define TS_MAXX 595
#define TS_MINY 305
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
#define TS_RES_OHMS 300

#define BUTTON_DEBOUNCE_MS 250
#define DRIVE_RC_REFRESH_MS 40 // 25Hz

// Touchscreen objects
Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);
TouchScreen grid = TouchScreen(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES_OHMS);

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
  Serial.println(y);
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
  Serial.begin(57600);
  Serial.print(F("Loading CIRC Wheele Display..."));

  screen.begin();
  screen.setRotation(3);
  draw_home_screen(screen);
  new_screen = current_screen = HOME_SCREEN;
  Serial.print(F("finished.\n"));
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
  unsigned long current_time;

  current_time = millis();
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
