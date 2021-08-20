#include "gui.h"
#include "../radio/radio.h"

#define THIS_PAGE E_PG_LORA

#define BUTTON_WIDTH 75
#define BUTTON_HEIGHT 30

#define INC_BUTTON_COORDS (gslc_tsRect){20,60,BUTTON_WIDTH,BUTTON_HEIGHT}
#define DEC_BUTTON_COORDS (gslc_tsRect){20,100,BUTTON_WIDTH,BUTTON_HEIGHT}
#define SAVE_BUTTON_COORDS (gslc_tsRect){20,140,BUTTON_WIDTH,BUTTON_HEIGHT}
#define REVERT_BUTTON_COORDS (gslc_tsRect){20,180,BUTTON_WIDTH,BUTTON_HEIGHT}
#define TEST_BUTTON_COORDS (gslc_tsRect){120,60,BUTTON_WIDTH,BUTTON_HEIGHT}
//#define SETUP_BUTTON_COORDS (gslc_tsRect){20,120,BUTTON_WIDTH,BUTTON_HEIGHT}

enum{
  E_ELEM_TITLE,
  E_ELEM_BTN_INC,
  E_ELEM_BTN_DEC,
  E_ELEM_BTN_SAVE,
  E_ELEM_BTN_REVERT,
  E_ELEM_BTN_TEST,
  MAX_ELEM
};

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];
static char test_buffer[16] = "Hello World    ";

// Button callbacks
static bool CbBtnTest(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
   if (eTouch == GSLC_TOUCH_UP_IN){
      Serial.println("Radio Test");
      radio_tx(test_buffer,16);
   }
   return true;
}

static bool CbBtnLora(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);
  int16_t nElemId = pElem->nId;

  if (eTouch == GSLC_TOUCH_UP_IN) {
    switch(nElemId){
      case E_ELEM_BTN_INC:
        Serial.println("Increment");
        break;

      case E_ELEM_BTN_DEC:
        Serial.println("Decrement");
        break;

      case E_ELEM_BTN_SAVE:
        Serial.println("Save");
        break;

      case E_ELEM_BTN_REVERT:
        Serial.println("Revert");
        break;
    }
  }
  return true;
}

void init_lora_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);

  // TITLE
  pElemRef = gslc_ElemCreateTxt(pGui,E_ELEM_TITLE,THIS_PAGE,
    TITLE_COORDS,(char*)"LoRa Setup",0,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLUE);

  // Increment Channel
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_INC,THIS_PAGE,
    INC_BUTTON_COORDS,(char*)"INC",0,E_FONT_BTN,&CbBtnLora);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Decrement Channel
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_DEC,THIS_PAGE,
    DEC_BUTTON_COORDS,(char*)"DEC",0,E_FONT_BTN,&CbBtnLora);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Store New Channel
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_SAVE,THIS_PAGE,
    SAVE_BUTTON_COORDS,(char*)"SAVE",0,E_FONT_BTN,&CbBtnLora);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Revert to Previous Channel
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_REVERT,THIS_PAGE,
    REVERT_BUTTON_COORDS,(char*)"REVERT",0,E_FONT_BTN,&CbBtnLora);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Test radio transmisson
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_TEST,THIS_PAGE,
    TEST_BUTTON_COORDS,(char*)"TEST",0,E_FONT_BTN,&CbBtnTest);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);
}
