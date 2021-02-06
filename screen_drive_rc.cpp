#include "screen.h"

#define CONTROL_STICK_PERSISTENCE_MS 250

#define CTRL_SURFACE_X 120
#define CTRL_SURFACE_Y 40
#define CTRL_SURFACE_WIDTH 195
#define CTRL_SURFACE_HEIGHT 190
#define CTRL_STICK_RADIUS 25
#define CTRL_STICK_X (CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2) - CTRL_STICK_RADIUS)
#define CTRL_STICK_Y (CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2) - CTRL_STICK_RADIUS)
#define ORIGIN_X_VERTICAL (CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2))
#define ORIGIN_Y_HORIZONTAL (CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2))

static Box back_button(10,5,30,20,SCREEN_COLOR_RED);
static Box auto_mode_button(10,85,100,40,SCREEN_COLOR_BLACK,true);
static Box manual_mode_button(10,135,100,40,SCREEN_COLOR_BLACK,true);
static Box joystick(CTRL_SURFACE_X,CTRL_SURFACE_Y,CTRL_SURFACE_WIDTH,CTRL_SURFACE_HEIGHT,SCREEN_COLOR_BLACK,true);
static Box control_stick(CTRL_STICK_X,CTRL_STICK_Y,CTRL_STICK_RADIUS*2,CTRL_STICK_RADIUS*2,SCREEN_COLOR_BLACK);

static unsigned long control_stick_time;
static unsigned long update_time;
static unsigned int manual_mode_enabled;

void draw_drive_rc_screen(Adafruit_ILI9341& screen){

  screen.fillScreen(ILI9341_DARKGREY);
  screen.setCursor(80,10);
  screen.setTextSize(2);
  screen.setTextColor(SCREEN_TITLE_COLOR);
  screen.print(F("WheelE RC Drive"));
  screen.fillRect(0,SCREEN_TITLE_BAR_Y,320,SCREEN_TITLE_BAR_HEIGHT,SCREEN_TITLE_BAR_COLOR);

  // buttons
  back_button.draw(screen);
  auto_mode_button.set_label(String("Auto"),SCREEN_COLOR_RED);
  auto_mode_button.draw(screen);
  manual_mode_button.set_label(String("Manual"),SCREEN_COLOR_RED);
  manual_mode_button.draw(screen);

  // joystick control area
  joystick.draw(screen);
  screen.fillCircle(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2),CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2),CTRL_STICK_RADIUS,SCREEN_COLOR_LIGHTGREY);
  screen.drawFastVLine(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2), CTRL_SURFACE_Y, CTRL_SURFACE_HEIGHT, SCREEN_COLOR_BLUE);
  screen.drawFastHLine(CTRL_SURFACE_X, CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2), CTRL_SURFACE_WIDTH, SCREEN_COLOR_BLUE);
}

int touch_drive_rc_screen(Adafruit_ILI9341& screen, TSPoint& p)
{
  unsigned int radius_x,radius_y,radius;
  int new_screen = DRIVE_RC_SCREEN; // default no change

  if(back_button.touched(p)){
    new_screen = HOME_SCREEN;
  }

  if(manual_mode_enabled){
    if(joystick.touched(p)){
      if(control_stick.touched(p)){
        control_stick.set_position(p.x - CTRL_STICK_RADIUS,p.y - CTRL_STICK_RADIUS);
        joystick.draw(screen);
        screen.drawFastVLine(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2), CTRL_SURFACE_Y, CTRL_SURFACE_HEIGHT, SCREEN_COLOR_BLUE);
        screen.drawFastHLine(CTRL_SURFACE_X, CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2), CTRL_SURFACE_WIDTH, SCREEN_COLOR_BLUE);

        if(p.x > ORIGIN_X_VERTICAL)
          radius_x = min(CTRL_SURFACE_X+CTRL_SURFACE_WIDTH-1 - p.x, CTRL_STICK_RADIUS);
        else
          radius_x = min(p.x - CTRL_SURFACE_X, CTRL_STICK_RADIUS);

        if(p.y > ORIGIN_Y_HORIZONTAL)
          radius_y = min(CTRL_SURFACE_Y+CTRL_SURFACE_HEIGHT-1 - p.y, CTRL_STICK_RADIUS);
        else
          radius_y = min(p.y - CTRL_SURFACE_Y, CTRL_STICK_RADIUS);

        radius = min(min(radius_x,radius_y),CTRL_STICK_RADIUS);
        screen.fillCircle(p.x,p.y,radius,SCREEN_COLOR_RED);
        screen.drawFastVLine(p.x, CTRL_SURFACE_Y, CTRL_SURFACE_HEIGHT, SCREEN_COLOR_YELLOW);
        screen.drawFastHLine(CTRL_SURFACE_X, p.y, CTRL_SURFACE_WIDTH, SCREEN_COLOR_YELLOW); 
        control_stick_time = millis();
      }
    }
  }

  if(auto_mode_button.touched(p)){
    manual_mode_enabled = 0;
    auto_mode_button.set_color(SCREEN_COLOR_YELLOW);
    auto_mode_button.draw(screen);
    manual_mode_button.set_color(SCREEN_COLOR_BLACK);
    manual_mode_button.draw(screen);    
    screen.fillCircle(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2),CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2),CTRL_STICK_RADIUS,SCREEN_COLOR_LIGHTGREY);
    screen.drawFastVLine(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2), CTRL_SURFACE_Y, CTRL_SURFACE_HEIGHT, SCREEN_COLOR_BLUE);
    screen.drawFastHLine(CTRL_SURFACE_X, CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2), CTRL_SURFACE_WIDTH, SCREEN_COLOR_BLUE);
  }

  if(manual_mode_button.touched(p)){
    manual_mode_enabled = 1;
    auto_mode_button.set_color(SCREEN_COLOR_BLACK);
    auto_mode_button.draw(screen);
    manual_mode_button.set_color(SCREEN_COLOR_YELLOW);
    manual_mode_button.draw(screen);    
    screen.fillCircle(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2),CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2),CTRL_STICK_RADIUS,SCREEN_COLOR_YELLOW);
    screen.drawFastVLine(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2), CTRL_SURFACE_Y, CTRL_SURFACE_HEIGHT, SCREEN_COLOR_BLUE);
    screen.drawFastHLine(CTRL_SURFACE_X, CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2), CTRL_SURFACE_WIDTH, SCREEN_COLOR_BLUE);
  }
  
  return new_screen;
}

void update_drive_rc_screen(Adafruit_ILI9341& screen){
  unsigned long current_time = millis();

  if(control_stick_time != 0){
    if((current_time - control_stick_time) > CONTROL_STICK_PERSISTENCE_MS){
      control_stick_time = 0;
      control_stick.set_position(CTRL_STICK_X,CTRL_STICK_Y);
      joystick.draw(screen);
      screen.fillCircle(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2),CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2),CTRL_STICK_RADIUS,SCREEN_COLOR_YELLOW);
      screen.drawFastVLine(CTRL_SURFACE_X + (CTRL_SURFACE_WIDTH/2), CTRL_SURFACE_Y, CTRL_SURFACE_HEIGHT, SCREEN_COLOR_BLUE);
      screen.drawFastHLine(CTRL_SURFACE_X, CTRL_SURFACE_Y + (CTRL_SURFACE_HEIGHT/2), CTRL_SURFACE_WIDTH, SCREEN_COLOR_BLUE);
    }
  }
}
