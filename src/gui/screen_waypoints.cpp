#include "gui.h"

#define THIS_PAGE E_PG_WAYPOINTS

#define BUTTON_WIDTH 75
#define BUTTON_HEIGHT 30
#define MAX_STR 16
#define MAX_ELEM  16
#define TBOX_ROWS 10
#define TBOX_COLS 10

#define GPS_LAT_COORDS (gslc_tsRect){100,180,200,20}
#define GPS_LON_COORDS (gslc_tsRect){100,200,200,20}
#define LST_WYPNTS_COORDS (gslc_tsRect){100,50,120,4*BUTTON_HEIGHT}
#define BTN_INS_COORDS (gslc_tsRect){20,50,75,75}
#define BTN_CONE_COORDS (gslc_tsRect){20,130,75,75}
#define BTN_TOGGLE_COORDS (gslc_tsRect){225,50,BUTTON_WIDTH,BUTTON_HEIGHT}
#define BTN_DEL_COORDS (gslc_tsRect){225,95,BUTTON_WIDTH,BUTTON_HEIGHT}
#define BTN_CLR_COORDS (gslc_tsRect){225,140,BUTTON_WIDTH,BUTTON_HEIGHT}

#define PRV_BUTTON_COORDS (gslc_tsRect){165,180,40,40}
#define NXT_BUTTON_COORDS (gslc_tsRect){230,180,40,40}

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];
static gslc_tsElemRef *pListBox;
static char s_lat[MAX_STR] = {0};
static char s_lon[MAX_STR] = {0};
static gslc_tsXTextbox s_Textbox;
static char s_acTextboxBuf[TBOX_ROWS*TBOX_COLS];

// Button callbacks
static bool CbBtnPrev(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_DOWN_IN) {
   }
   return true;
}

static bool CbBtnNext(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_DOWN_IN) {
   }
   return true;
}

void init_waypoints_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);

  // TITLE
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    TITLE_COORDS,(char*)"Waypoints",0,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLUE);

  // GPS
  snprintf(s_lat,MAX_STR,"LAT: -");
  pElemRef = gslc_ElemCreateTxt(pGui,E_ELEM_WYPT_ID_GPS_LAT,THIS_PAGE,
    GPS_LAT_COORDS,s_lat,MAX_STR,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_BOT_LEFT);

  snprintf(s_lon,MAX_STR,"LON: -");
  pElemRef = gslc_ElemCreateTxt(pGui,E_ELEM_WYPT_ID_GPS_LON,THIS_PAGE,
    GPS_LON_COORDS,s_lon,MAX_STR,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_BOT_LEFT);

  // Insert waypoint
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_INS,THIS_PAGE,
    BTN_INS_COORDS,(char*)"WAYPNT",0,E_FONT_BTN,NULL);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Insert Cone
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_CONE,THIS_PAGE,
    BTN_CONE_COORDS,(char*)"CONE",0,E_FONT_BTN,NULL);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_ORANGE,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Toggle waypoint type (cone/intermediate)
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_TOGGLE,THIS_PAGE,
    BTN_TOGGLE_COORDS,(char*)"TOGGLE",0,E_FONT_BTN,NULL);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Delete waypoint
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_DEL,THIS_PAGE,
    BTN_DEL_COORDS,(char*)"DELETE",0,E_FONT_BTN,NULL);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Clear waypoints
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_CLR,THIS_PAGE,
    BTN_CLR_COORDS,(char*)"CLEAR",0,E_FONT_BTN,NULL);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_RED,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Create textbox
  pListBox = gslc_ElemXTextboxCreate(pGui,E_ELEM_WYPT_ID_LST_WYPNTS,THIS_PAGE,
    &s_Textbox,LST_WYPNTS_COORDS,E_FONT_BTN,(char*)&s_acTextboxBuf,TBOX_ROWS,TBOX_COLS);
  gslc_ElemXTextboxWrapSet(pGui,pListBox,true);
  gslc_ElemSetCol(pGui,pListBox,GSLC_COL_BLUE_LT2,GSLC_COL_BLACK,GSLC_COL_GRAY_DK3);

  // testing
  gslc_ElemXTextboxAdd(pGui,pListBox,(char*)"Welcome\n");
  gslc_ElemXTextboxAdd(pGui,pListBox,(char*)"Hi \n");
  gslc_ElemXTextboxAdd(pGui,pListBox,(char*)"192.30.70\n");

  // Next waypoint
  // pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_NXT,THIS_PAGE,
  //   NXT_BUTTON_COORDS,(char*)">",0,E_FONT_DISPLAY,&CbBtnNext);
  // gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  // gslc_ElemSetRoundEn(pGui,pElemRef,true);
  //
  // // Previous waypoint
  // pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_WYPT_ID_BTN_PRV,THIS_PAGE,
  //   PRV_BUTTON_COORDS,(char*)"<",0,E_FONT_DISPLAY,&CbBtnPrev);
  // gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  // gslc_ElemSetRoundEn(pGui,pElemRef,true);
}
