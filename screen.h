#ifndef SCREEN_H_
#define SCREEN_H_

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

#ifdef _ADAFRUIT_ILI9341H_
#define SCREEN_COLOR_BLACK     ILI9341_BLACK
#define SCREEN_COLOR_LIGHTGREY ILI9341_LIGHTGREY
#define SCREEN_COLOR_DARKGREY  ILI9341_DARKGREY
#define SCREEN_COLOR_BLUE      ILI9341_BLUE
#define SCREEN_COLOR_RED       ILI9341_RED
#define SCREEN_COLOR_WHITE     ILI9341_WHITE
#define SCREEN_COLOR_GREEN     ILI9341_GREEN
#endif

#define SCREEN_TITLE_COLOR SCREEN_COLOR_BLUE
#define SCREEN_TITLE_BAR_Y 30
#define SCREEN_TITLE_BAR_HEIGHT 5
#define SCREEN_TITLE_BAR_COLOR SCREEN_COLOR_RED

enum {
  HOME_SCREEN,
  SHUTDOWN_SCREEN,
  WAYPOINTS_SCREEN
}screen_id;

class Box{
public:
  Box(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color, bool rounded = false, uint16_t radius = 5){
    this->x = x0; this->y = y0; this->w = w; this->h = h; this->color = color; this->rounded = rounded, this->radius = radius;
  }
  ~Box(){}

  void draw(Adafruit_ILI9341& screen){
    int l;
    if(!rounded){
      screen.fillRect(x,y,w,h,color);
      screen.drawRect(x,y,w,h,SCREEN_COLOR_BLACK);
    }
    else {
      screen.fillRoundRect(x,y,w,h,radius,color);
      screen.drawRoundRect(x,y,w,h,radius,SCREEN_COLOR_BLUE);
    }

    l = label.length();
    if(l!=0){
      int x_pad = w/2 - (12*l/2);
      int y_pad = h/2 - 8;
      screen.setTextColor(label_color);
      screen.setTextSize(2);
      screen.setCursor(x+x_pad,y+y_pad);
      screen.print(label);      
    }
  }

  bool touched(TSPoint& p){
    if((p.x > x)&&(p.x < x + w)){
      if((p.y > y)&&(p.y < y + h)){
        return true;
      }
    }
    return false;
  }

  void set_label(String label, uint16_t color = SCREEN_COLOR_BLACK){
    this->label = label;
    this->label_color = color;
  }

protected:
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  uint16_t color;
  bool rounded;
  uint16_t radius;
  String label;
  uint16_t label_color;
};

class Numberbox: public Box{
public:
  Numberbox(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color, uint16_t max_value = 65535):Box(x0,y0,w,h,color,false){
    this->value = 0;
    this->max_value = max_value;
  }
  ~Numberbox(){}
  
  void highlight(Adafruit_ILI9341& screen){
    if(!rounded){
      screen.drawRect(x,y,w,h,SCREEN_COLOR_RED);
    }
  }

  void clear_highlight(Adafruit_ILI9341& screen){
    if(!rounded){
      screen.drawRect(x,y,w,h,SCREEN_COLOR_BLACK);
    }
  }

  void clear_value(){
    value = 0;
  }

  void update_value(uint16_t number){
    uint16_t y;
    
    if(value == 0)
      y = number;
    else if(value < 100)
      y = 10*value + number;
    else
      y = value;

    if(y <= max_value)
      value = y;
  }

  void print_value(Adafruit_ILI9341& screen){
    int offset;

    if(value < 10)
      offset = 25;
    else if(value < 100)
      offset = 15;
    else
      offset = 3;
      
    this->draw(screen);
    screen.setTextColor(SCREEN_COLOR_BLACK);
    screen.setTextSize(2);
    screen.setCursor(x+offset,y+3);
    screen.print(value);
  }

private:
  uint16_t value;
  uint16_t max_value;
};

void draw_home_screen(Adafruit_ILI9341& screen);
int touch_home_screen(Adafruit_ILI9341& screen, TSPoint& p);

void draw_waypoints_screen(Adafruit_ILI9341& screen);
int touch_waypoints_screen(Adafruit_ILI9341& screen, TSPoint& p);
void update_waypoints_screen(Adafruit_ILI9341& screen);

void draw_shutdown_screen(Adafruit_ILI9341& screen);
int touch_shutdown_screen(Adafruit_ILI9341& screen, TSPoint& p);
void update_shutdown_screen(Adafruit_ILI9341& screen);
#endif
