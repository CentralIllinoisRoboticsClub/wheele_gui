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
  E_PG_CTRL,
  MAX_PAGE
};

enum{
   E_ELEM_INFO_ID_VBATT
};

enum{
   E_ELEM_CTRL_ID_WHEELE_MODE,
   E_ELEM_CTRL_ID_NAV_STATE,
   E_ELEM_CTRL_ID_CUR_HEAD,
   E_ELEM_CTRL_ID_TRGT_HEAD,
   E_ELEM_CTRL_ID_TRGT_WYPT,
   E_ELEM_CTRL_ID_TRGT_DIST,
   E_ELEM_CTRL_ID_ROS_BOX,
   E_ELEM_CTRL_ID_GPS_BOX
};

enum{
  E_ELEM_WYPT_ID_TITLE,
  E_ELEM_WYPT_ID_GPS_LAT,
  E_ELEM_WYPT_ID_GPS_LON,
  E_ELEM_WYPT_ID_LST_WYPNTS,
  E_ELEM_WYPT_ID_BTN_CLR,
  E_ELEM_WYPT_ID_BTN_NXT,
  E_ELEM_WYPT_ID_BTN_PRV,
  E_ELEM_WYPT_ID_BTN_DEL,
  E_ELEM_WYPT_ID_BTN_INS,
  E_ELEM_WYPT_ID_BTN_CONE,
  E_ELEM_WYPT_ID_BTN_TOGGLE
};

void gui_init(gslc_tsGui *pGUI);
gslc_tsGui *gui_get_ref(void);

void init_base_screen(gslc_tsGui* pGui);
void init_home_screen(gslc_tsGui* pGui);
void init_setup_screen(gslc_tsGui* pGui);
void init_lora_screen(gslc_tsGui* pGui);
void init_shutdown_screen(gslc_tsGui* pGui);
void init_waypoints_screen(gslc_tsGui* pGui);
void init_info_screen(gslc_tsGui* pGui);
void init_control_screen(gslc_tsGui* pGui);
#endif
