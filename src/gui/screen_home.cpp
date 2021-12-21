#include "gui.h"

#define THIS_PAGE E_PG_HOME
#define MAX_ELEM_PG_HOME   15 // Max # of elements on main page

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40

#define SETUP_BUTTON_COORDS (gslc_tsRect){20,60,BUTTON_WIDTH,BUTTON_HEIGHT}
#define WAYPOINTS_BUTTON_COORDS (gslc_tsRect){20,120,BUTTON_WIDTH,BUTTON_HEIGHT}
#define CONTROL_BUTTON_COORDS (gslc_tsRect){160,60,BUTTON_WIDTH,BUTTON_HEIGHT}
#define SHUTDOWN_BUTTON_COORDS (gslc_tsRect){160,120,BUTTON_WIDTH,BUTTON_HEIGHT}
#define INFO_BUTTON_COORDS (gslc_tsRect){160,180,BUTTON_WIDTH,BUTTON_HEIGHT}

static gslc_tsElem    m_asHomeElem[MAX_ELEM_PG_HOME];
static gslc_tsElemRef m_asHomeElemRef[MAX_ELEM_PG_HOME];

// Button callbacks
bool CbBtnSetup(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_SETUP);
   }
   return true;
}

bool CbBtnWaypoints(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_WAYPOINTS);
   }
   return true;
}

bool CbBtnControl(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_CTRL);
   }
   return true;
}

bool CbBtnShutdown(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_SHUTDOWN);
   }
   return true;
}

bool CbBtnInfo(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_INFO);
   }
   return true;
}

void init_home_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,E_PG_HOME,m_asHomeElem,MAX_ELEM_PG_HOME,m_asHomeElemRef,MAX_ELEM_PG_HOME);

  // TITLE
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    TITLE_COORDS,(char*)"Wheele Control Panel",0,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLUE);

  // SETUP
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    SETUP_BUTTON_COORDS,(char*)"SETUP",0,E_FONT_BTN,&CbBtnSetup);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // WAYPOINTS
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    WAYPOINTS_BUTTON_COORDS,(char*)"WAYPOINTS",0,E_FONT_BTN,&CbBtnWaypoints);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // CONTROL
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    CONTROL_BUTTON_COORDS,(char*)"CONTROL",0,E_FONT_BTN,&CbBtnControl);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // SHUTDOWN
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    SHUTDOWN_BUTTON_COORDS,(char*)"SHUTDOWN",0,E_FONT_BTN,&CbBtnShutdown);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // INFO
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    INFO_BUTTON_COORDS,(char*)"INFO",0,E_FONT_BTN,&CbBtnInfo);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);
}
