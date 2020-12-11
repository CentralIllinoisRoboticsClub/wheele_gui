#include "screen.h"

#define COUNTDOWN_TIMER_RESET 5
#define COUNTDOWN_TIMER_DELAY_MS 1000

static Box back_button(10,5,30,20,SCREEN_COLOR_RED);
static Box confirm_button(190,85,100,40,SCREEN_COLOR_BLACK);
static Box cancel_button(190,135,100,40,SCREEN_COLOR_BLACK);
static Box countdown_box(25,75,125,125,SCREEN_COLOR_BLACK,true,10);

static int countdown_timer = COUNTDOWN_TIMER_RESET;
static unsigned long update_time;

void draw_shutdown_screen(Adafruit_ILI9341& screen){

  screen.fillScreen(ILI9341_DARKGREY);
  screen.setCursor(80,10);
  screen.setTextSize(2);
  screen.setTextColor(SCREEN_TITLE_COLOR);
  screen.print(F("WheelE Shutdown"));
  screen.fillRect(0,SCREEN_TITLE_BAR_Y,320,SCREEN_TITLE_BAR_HEIGHT,SCREEN_TITLE_BAR_COLOR);

  // buttons
  back_button.draw(screen);
  confirm_button.set_label(String("Confirm"),SCREEN_COLOR_WHITE);
  confirm_button.draw(screen);
  cancel_button.set_label(String("Cancel"),SCREEN_COLOR_WHITE);
  cancel_button.draw(screen);

  // countdown timer
  if(countdown_timer != 0){
    countdown_box.set_label(String(countdown_timer),SCREEN_COLOR_RED);
    countdown_box.draw(screen);
    update_time = millis();
  }
}

int touch_shutdown_screen(Adafruit_ILI9341& screen, TSPoint& p)
{
  int new_screen = SHUTDOWN_SCREEN; // default no change
  
  if(back_button.touched(p) || cancel_button.touched(p)){
    countdown_timer = COUNTDOWN_TIMER_RESET;
    new_screen = HOME_SCREEN;
  }

  if(confirm_button.touched(p)){
    countdown_timer = 0;
  }

  return new_screen;
}

void update_shutdown_screen(Adafruit_ILI9341& screen){
  unsigned long current_time = millis();

  if((current_time - update_time) > COUNTDOWN_TIMER_DELAY_MS){
    if(countdown_timer > 1){
      countdown_timer--;
      countdown_box.set_label(String(countdown_timer),SCREEN_COLOR_RED);
      countdown_box.draw(screen);
      update_time = millis();
    }
    else{
      countdown_box.set_label(String("SHUTDOWN!"),SCREEN_COLOR_RED);
      countdown_box.draw(screen);      
    }
  }  
}
