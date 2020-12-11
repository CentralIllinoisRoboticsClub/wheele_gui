#include "screen.h"

#define KEYPAD_BUTTON_SIZE_X 45
#define KEYPAD_BUTTON_SIZE_Y 45
#define KEYPAD_BUTTON_PADDING 2
#define KEYPAD_BUTTON_RADIUS 5

#define COORDINATE_BOX_SIZE_X 40
#define COORDINATE_BOX_SIZE_Y 20

#define KEYPAD_X_START 5
#define KEYPAD_Y_START 45

#define ACTIVE_BOX_NONE 255
#define ROUNDED_BOX true

#define CONFIRM_TIMER_INIT 4
#define CONFIRM_DELAY_MS 1000

static Numberbox waypoint_box[]=
{
  // latitude
  {175,50,COORDINATE_BOX_SIZE_X,COORDINATE_BOX_SIZE_Y,SCREEN_COLOR_LIGHTGREY,360}, // degrees
  {175 + COORDINATE_BOX_SIZE_X + 5,50,COORDINATE_BOX_SIZE_X,COORDINATE_BOX_SIZE_Y,SCREEN_COLOR_LIGHTGREY,60}, // minutes
  {175 + 2*COORDINATE_BOX_SIZE_X + 10,50,COORDINATE_BOX_SIZE_X,COORDINATE_BOX_SIZE_Y,SCREEN_COLOR_LIGHTGREY,60}, // seconds
  
  // longitude
  {175,80,COORDINATE_BOX_SIZE_X,COORDINATE_BOX_SIZE_Y,SCREEN_COLOR_LIGHTGREY,360}, // degrees
  {175 + COORDINATE_BOX_SIZE_X + 5,80,COORDINATE_BOX_SIZE_X,COORDINATE_BOX_SIZE_Y,SCREEN_COLOR_LIGHTGREY,60}, // minutes
  {175 + 2*COORDINATE_BOX_SIZE_X + 10,80,COORDINATE_BOX_SIZE_X,COORDINATE_BOX_SIZE_Y,SCREEN_COLOR_LIGHTGREY,60}, //seconds
};
#define NUM_WAYPOINT_BOXES sizeof(waypoint_box)/sizeof(Numberbox)


static Box back_button(10,5,30,20,SCREEN_COLOR_RED);
static Box confirm_button(190,125,100,40,SCREEN_COLOR_BLACK,ROUNDED_BOX);
static Box cancel_button(190,175,100,40,SCREEN_COLOR_BLACK,ROUNDED_BOX);
static Box confirm_message(70,50,150,150,SCREEN_COLOR_BLACK,ROUNDED_BOX);
static int active_box;
static int confirm_timer;
static unsigned long update_time;

void draw_waypoints_screen(Adafruit_ILI9341& screen)
{
  int i,j;
  screen.fillScreen(SCREEN_COLOR_DARKGREY);
  screen.setCursor(70,10);
  screen.setTextSize(2);
  screen.setTextColor(SCREEN_TITLE_COLOR);
  screen.print(F("Enter Waypoints"));
  screen.fillRect(0,SCREEN_TITLE_BAR_Y,320,SCREEN_TITLE_BAR_HEIGHT,SCREEN_TITLE_BAR_COLOR);

  // keypad
  {
    int keypad_number;
    int x,y;

    screen.setTextSize(2);
    screen.setTextColor(SCREEN_COLOR_WHITE);

    keypad_number = 1;
    for(i=0;i<3;i++){
      for(j=0;j<3;j++){
        x = KEYPAD_X_START + (j*(KEYPAD_BUTTON_SIZE_X+KEYPAD_BUTTON_PADDING));
        y = KEYPAD_Y_START + (i*(KEYPAD_BUTTON_SIZE_Y+KEYPAD_BUTTON_PADDING));
        screen.fillRoundRect(x, y, KEYPAD_BUTTON_SIZE_X, KEYPAD_BUTTON_SIZE_Y, KEYPAD_BUTTON_RADIUS, SCREEN_COLOR_BLACK);
        screen.drawRoundRect(x, y, KEYPAD_BUTTON_SIZE_X, KEYPAD_BUTTON_SIZE_Y, KEYPAD_BUTTON_RADIUS, SCREEN_COLOR_BLUE);

        screen.setCursor(x + KEYPAD_BUTTON_SIZE_X/3 + 3, y + KEYPAD_BUTTON_SIZE_Y/3);
        screen.print(keypad_number);
        keypad_number++;
      }
    }

    x = KEYPAD_X_START + (1*(KEYPAD_BUTTON_SIZE_X+KEYPAD_BUTTON_PADDING));
    y = KEYPAD_Y_START + (3*(KEYPAD_BUTTON_SIZE_Y+KEYPAD_BUTTON_PADDING));
    keypad_number = 0;
    screen.fillRoundRect(x, y, KEYPAD_BUTTON_SIZE_X, KEYPAD_BUTTON_SIZE_Y, KEYPAD_BUTTON_RADIUS, SCREEN_COLOR_BLACK);
    screen.drawRoundRect(x, y, KEYPAD_BUTTON_SIZE_X, KEYPAD_BUTTON_SIZE_Y, KEYPAD_BUTTON_RADIUS, SCREEN_COLOR_BLUE);
    screen.setCursor(x + KEYPAD_BUTTON_SIZE_X/3 + 3, y + KEYPAD_BUTTON_SIZE_Y/3);
    screen.print(keypad_number);
  }

  // lat/long
  for(i=0;i<NUM_WAYPOINT_BOXES;i++){
    waypoint_box[i].draw(screen);
    waypoint_box[i].print_value(screen);
  }

  screen.setTextColor(SCREEN_COLOR_BLACK);
  screen.setTextSize(2);
  screen.setCursor(160,53);
  screen.print("N");
  screen.setCursor(160,83);
  screen.print("W");

  back_button.draw(screen);
  confirm_button.set_label(String("Confirm"),SCREEN_COLOR_WHITE);
  confirm_button.draw(screen);
  cancel_button.set_label(String("Cancel"),SCREEN_COLOR_WHITE);
  cancel_button.draw(screen);

  active_box = ACTIVE_BOX_NONE;
  confirm_timer = 0;
}

int touch_waypoints_screen(Adafruit_ILI9341& screen, TSPoint& p)
{
  int x,y,i,j;
  uint16_t keypad_number;
  int new_screen = WAYPOINTS_SCREEN; // default no change

  // check if a waypoint box was touched
  for(i=0;i<NUM_WAYPOINT_BOXES;i++){
    if(waypoint_box[i].touched(p)){
      if(active_box != ACTIVE_BOX_NONE){
        waypoint_box[active_box].clear_highlight(screen);
      }
      active_box = i;
      waypoint_box[i].draw(screen);
      waypoint_box[i].highlight(screen);
      waypoint_box[i].clear_value();
    }
  }

  // check if keypad was touched
  for(i=0;i<4;i++){
    for(j=0;j<3;j++){
      x = KEYPAD_X_START + (j*(KEYPAD_BUTTON_SIZE_X+KEYPAD_BUTTON_PADDING));
      y = KEYPAD_Y_START + (i*(KEYPAD_BUTTON_SIZE_Y+KEYPAD_BUTTON_PADDING));
      if((p.x > x)&&(p.x < (x + KEYPAD_BUTTON_SIZE_X))){
        if((p.y > y)&&(p.y < (y + KEYPAD_BUTTON_SIZE_Y))){
          if(active_box != ACTIVE_BOX_NONE){
            if(i < 3){
              keypad_number = (i*3) + (j+1);
            }
            else {
              if(j == 1)
                keypad_number = 0;
            }
            waypoint_box[active_box].update_value(keypad_number);
            waypoint_box[active_box].print_value(screen);
            waypoint_box[active_box].highlight(screen);
          }         
        }
      }
    }
  }

  if(confirm_button.touched(p)){
    confirm_message.set_label(String("Confirmed!"),SCREEN_COLOR_GREEN);
    confirm_message.draw(screen);
    confirm_timer = CONFIRM_TIMER_INIT;
  }

  if(cancel_button.touched(p)){
    active_box = ACTIVE_BOX_NONE;
    for(i=0;i<NUM_WAYPOINT_BOXES;i++){
      waypoint_box[i].draw(screen);
      waypoint_box[i].clear_highlight(screen);
      waypoint_box[i].clear_value();
      waypoint_box[i].print_value(screen);
    }    
  }
  
  if(back_button.touched(p)){
    new_screen = HOME_SCREEN;
  }
  
  return new_screen;
}

void update_waypoints_screen(Adafruit_ILI9341& screen){
  static bool confirm_active = false;
  unsigned long current_time = millis();

  if(confirm_timer != 0){
    confirm_active = true;
    if((current_time - update_time) > CONFIRM_DELAY_MS)
    {
      update_time = current_time;
      confirm_timer--;
    }
  }
  else{
    if(confirm_active){
      confirm_active = false;
      draw_waypoints_screen(screen);
    }
  }
}
