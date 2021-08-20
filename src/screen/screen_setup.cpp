#include "gui.h"

#define THIS_PAGE E_PG_SETUP
#define MAX_ELEM 5

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40

#define LORA_BUTTON_COORDS (gslc_tsRect){20,60,BUTTON_WIDTH,BUTTON_HEIGHT}
#define BAGFILE_BUTTON_COORDS (gslc_tsRect){20,120,BUTTON_WIDTH,BUTTON_HEIGHT}

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];

// Button callbacks
static bool CbBtnLora(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_LORA);
   }
   return true;
}

static bool CbBtnBagfile(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   gslc_tsGui *pGui = (gslc_tsGui*)(pvGui);
   if (eTouch == GSLC_TOUCH_UP_IN) {
      gslc_SetPageCur(pGui,E_PG_BAGFILE);
   }
   return true;
}

void init_setup_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);

  // TITLE
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    TITLE_COORDS,(char*)"Wheele Setup",0,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLUE);

  // LoRa
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    LORA_BUTTON_COORDS,(char*)"LoRa",0,E_FONT_BTN,&CbBtnLora);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // BAGFILE
  pElemRef = gslc_ElemCreateBtnTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    BAGFILE_BUTTON_COORDS,(char*)"BAGFILE",0,E_FONT_BTN,&CbBtnBagfile);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);
}
