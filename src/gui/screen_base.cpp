#include "gui.h"

#define THIS_PAGE E_PG_BASE
#define MAX_ELEM  15

#define BUTTON_WIDTH 30
#define BUTTON_HEIGHT 20

#define HOME_BUTTON_COORDS (gslc_tsRect){10,5,BUTTON_WIDTH,BUTTON_HEIGHT}

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];

// Button callbacks
bool CbBtnHome(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
  if (eTouch == GSLC_TOUCH_UP_IN) {
    gslc_SetPageCur(pGui,E_PG_HOME);
  }
  return true;
}

void init_base_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);
  gslc_SetPageBase(pGui,E_PG_BASE);

  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    HOME_BUTTON_COORDS,(char*)"",0,E_FONT_BTN,&CbBtnHome);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_RED,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  pElemRef = gslc_ElemCreateLine(pGui,GSLC_ID_AUTO,THIS_PAGE,0,30,pGui->nDispW,30);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_RED,GSLC_COL_BLUE,GSLC_COL_YELLOW);
}
