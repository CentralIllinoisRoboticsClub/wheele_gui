#include "screen.h"

#define BUTTON_SIZE_X 120
#define BUTTON_SIZE_Y 40
#define BUTTON_RADIUS 5

#define SHUTDOWN_BUTTON_X 20
#define SHUTDOWN_BUTTON_Y 60
#define WAYPOINTS_BUTTON_X 20
#define WAYPOINTS_BUTTON_Y 120
#define START_BUTTON_X 20
#define START_BUTTON_Y 180

static Box shutdown_button(SHUTDOWN_BUTTON_X,SHUTDOWN_BUTTON_Y,BUTTON_SIZE_X,BUTTON_SIZE_Y,SCREEN_COLOR_BLACK, true);
static Box waypoints_button(WAYPOINTS_BUTTON_X,WAYPOINTS_BUTTON_Y,BUTTON_SIZE_X,BUTTON_SIZE_Y,SCREEN_COLOR_BLACK, true);
static Box path_ready_button(START_BUTTON_X,START_BUTTON_Y,BUTTON_SIZE_X,BUTTON_SIZE_Y,SCREEN_COLOR_BLACK, true);

void draw_home_screen(Adafruit_ILI9341& screen){

  screen.fillScreen(ILI9341_DARKGREY);
  screen.setCursor(40,10);
  screen.setTextSize(2);
  screen.setTextColor(SCREEN_TITLE_COLOR);
  screen.print(F("WheelE Control Panel"));
  screen.fillRect(0,SCREEN_TITLE_BAR_Y,320,SCREEN_TITLE_BAR_HEIGHT,SCREEN_TITLE_BAR_COLOR);

  // shutdown button
  shutdown_button.set_label(String("Shutdown"),SCREEN_COLOR_RED);
  shutdown_button.draw(screen);
  
  // enter waypoint button
  waypoints_button.set_label(String("Waypoints"),SCREEN_COLOR_RED);
  waypoints_button.draw(screen);

  // start journey
  path_ready_button.set_label(String("Path"),SCREEN_COLOR_RED);
  path_ready_button.draw(screen);
}

int touch_home_screen(Adafruit_ILI9341& screen, TSPoint& p){
  int new_screen = HOME_SCREEN; // default no change
  
  // shutdown button
  if(shutdown_button.touched(p)){
      new_screen = SHUTDOWN_SCREEN;
  }
  
  // enter waypoint button
  if(waypoints_button.touched(p)){
      new_screen = WAYPOINTS_SCREEN;
  } 

  return new_screen;
}
