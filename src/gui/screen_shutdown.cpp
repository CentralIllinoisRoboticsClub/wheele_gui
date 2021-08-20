#include "gui.h"


#define THIS_PAGE E_PG_SHUTDOWN
#define MAX_ELEM_PG_SHUTDOWN  10 // Max # of elements on main page
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40

#define CONFIRM_BUTTON_COORDS (gslc_tsRect){190,85,BUTTON_WIDTH,BUTTON_HEIGHT}
#define CANCEL_BUTTON_COORDS (gslc_tsRect){190,135,BUTTON_WIDTH,BUTTON_HEIGHT}

static gslc_tsElem    m_asShutdownElem[MAX_ELEM_PG_SHUTDOWN];
static gslc_tsElemRef m_asShutdownElemRef[MAX_ELEM_PG_SHUTDOWN];

// Button callbacks
bool CbBtnConfirm(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      Serial.println("Confirm button pressed");
   }
   return true;
}

bool CbBtnCancel(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      Serial.println("Cancel button pressed");
   }
   return true;
}

void init_shutdown_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,E_PG_SHUTDOWN,
    m_asShutdownElem,MAX_ELEM_PG_SHUTDOWN,
    m_asShutdownElemRef,MAX_ELEM_PG_SHUTDOWN);

  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    CONFIRM_BUTTON_COORDS,(char*)"Confirm",0,E_FONT_BTN,&CbBtnConfirm);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    CANCEL_BUTTON_COORDS,(char*)"Cancel",0,E_FONT_BTN,&CbBtnCancel);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

}
