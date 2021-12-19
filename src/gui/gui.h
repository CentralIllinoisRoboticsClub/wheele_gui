#ifndef _GUI_H_
#define _GUI_H_

#include <Adafruit_GFX.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <GUIslice.h>
#include <GUIslice_ex.h>
#include <GUIslice_drv.h>
#include "../../git-version.h"

#define MAX_ELEM_PG_DRIVE_RC  10 // Max # of elements on main page
#define MAX_ELEM_PG_WAYPOINTS 10 // Max # of elements on main page
#define TITLE_COORDS (gslc_tsRect){75,0,300,30}
#define GUI_COL_BKGND GSLC_COL_GRAY_LT3

enum{
  E_FONT_BTN,
  E_FONT_TITLE,
  E_FONT_DISPLAY,
  E_FONT_LIST,
  MAX_FONT
};

enum{
  E_PG_BASE,
  E_PG_HOME,
  E_PG_SETUP,
  E_PG_WAYPOINTS,
  E_PG_CONTROL,
  E_PG_SHUTDOWN,
  E_PG_LORA,
  E_PG_BAGFILE,
  E_PG_CLEAR_WAYPOINTS,
  E_PG_INFO,
  MAX_PAGE
};

void gui_init(gslc_tsGui *pGUI);
void gui_update(void);
void update_info_screen(gslc_tsGui* pGui, uint32_t vbatt);

void init_base_screen(gslc_tsGui* pGui);
void init_home_screen(gslc_tsGui* pGui);
void init_setup_screen(gslc_tsGui* pGui);
void init_lora_screen(gslc_tsGui* pGui);
void init_shutdown_screen(gslc_tsGui* pGui);
void init_waypoints_screen(gslc_tsGui* pGui);
void init_info_screen(gslc_tsGui* pGui);
#endif
