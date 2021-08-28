#include "gui.h"
#include "../radio/radio.h"

#define THIS_PAGE E_PG_LORA

#define BUTTON_WIDTH 75
#define BUTTON_HEIGHT 30
#define MAX_STR 15
#define MIN_LORA_CHAN 0
#define MAX_LORA_CHAN 12

#define CHAN_TXT_COORDS (gslc_tsRect){140,140,BUTTON_WIDTH,BUTTON_HEIGHT}
#define FREQ_TXT_COORDS (gslc_tsRect){140,170,BUTTON_WIDTH,BUTTON_HEIGHT}
#define CHAN_BOX_COORDS (gslc_tsRect){130,50,95,90}
#define INC_BUTTON_COORDS (gslc_tsRect){230,50,40,40}
#define DEC_BUTTON_COORDS (gslc_tsRect){230,100,40,40}
#define SAVE_BUTTON_COORDS (gslc_tsRect){20,50,BUTTON_WIDTH,BUTTON_HEIGHT}
#define REVERT_BUTTON_COORDS (gslc_tsRect){20,90,BUTTON_WIDTH,BUTTON_HEIGHT}

#define TEST_BUTTON_COORDS (gslc_tsRect){20,140,BUTTON_WIDTH,2*BUTTON_HEIGHT+10}
//#define SETUP_BUTTON_COORDS (gslc_tsRect){20,120,BUTTON_WIDTH,BUTTON_HEIGHT}

enum{
  E_ELEM_TITLE,
  E_ELEM_TXT_CHAN,
  E_ELEM_TXT_FREQ,
  E_ELEM_BOX_CHAN,
  E_ELEM_BTN_INC,
  E_ELEM_BTN_DEC,
  E_ELEM_BTN_SAVE,
  E_ELEM_BTN_REVERT,
  E_ELEM_BTN_TEST,
  MAX_ELEM
};

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];
static gslc_tsElemRef *pChannelBox;
static gslc_tsElemRef *pFreqBox;
static int channel = 0;
static char test_buffer[16] = "Hello World    ";
static char acTxtChannel[MAX_STR] = {0};
static char acTxtFreq[MAX_STR] = {0};

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
  float freq;
  char acTxt[MAX_STR] = {0};

  if (eTouch == GSLC_TOUCH_DOWN_IN) {
    switch(nElemId){
      case E_ELEM_BTN_INC:
        if(channel < MAX_LORA_CHAN){
           channel++;
           freq = radio_get_chan_center_freq(channel);
           radio_set_frequency(freq);
           snprintf(acTxt,MAX_STR,"%i",channel);
           gslc_ElemSetTxtStr(pGui,pChannelBox,acTxt);
           snprintf(acTxt,MAX_STR,"%d.%02d MHz",(int)freq,(int)(freq*100)%100);
           gslc_ElemSetTxtStr(pGui,pFreqBox,acTxt);
           Serial.println("Increment");
        }
        break;

      case E_ELEM_BTN_DEC:
        if(channel > MIN_LORA_CHAN){
           channel--;
           freq = radio_get_chan_center_freq(channel);
           radio_set_frequency(freq);
           snprintf(acTxt,MAX_STR,"%i",channel);
           gslc_ElemSetTxtStr(pGui,pChannelBox,acTxt);
           snprintf(acTxt,MAX_STR,"%d.%02d MHz",(int)freq,(int)(freq*100)%100);
           gslc_ElemSetTxtStr(pGui,pFreqBox,acTxt);
        }
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
  float freq;
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);

  // TITLE
  pElemRef = gslc_ElemCreateTxt(pGui,E_ELEM_TITLE,THIS_PAGE,
    TITLE_COORDS,(char*)"LoRa Setup",0,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLUE);

  // Channel
  pElemRef = gslc_ElemCreateTxt(pGui,E_ELEM_TXT_CHAN,THIS_PAGE,
    CHAN_TXT_COORDS,(char*)"CHANNEL",0,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_MID);

  freq = radio_get_chan_center_freq(channel);
  snprintf(acTxtFreq,MAX_STR,"%d.%02d MHz",(int)freq,(int)(freq*100)%100);
  pFreqBox = gslc_ElemCreateTxt(pGui,E_ELEM_TXT_FREQ,THIS_PAGE,
    FREQ_TXT_COORDS,acTxtFreq,MAX_STR,E_FONT_TITLE);
  gslc_ElemSetCol(pGui,pFreqBox,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pFreqBox,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pFreqBox,GSLC_ALIGN_TOP_MID);

  snprintf(acTxtChannel,MAX_STR,"%i",channel);
  pChannelBox = gslc_ElemCreateTxt(pGui,E_ELEM_BOX_CHAN,THIS_PAGE,
    CHAN_BOX_COORDS,acTxtChannel,MAX_STR,E_FONT_DISPLAY);
  gslc_ElemSetCol(pGui,pChannelBox,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetTxtCol(pGui,pChannelBox,GSLC_COL_WHITE);
  gslc_ElemSetTxtAlign(pGui,pChannelBox,GSLC_ALIGN_MID_MID);
  gslc_ElemSetRoundEn(pGui,pChannelBox,true);

  // Increment Channel
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_INC,THIS_PAGE,
    INC_BUTTON_COORDS,(char*)"+",0,E_FONT_DISPLAY,&CbBtnLora);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);

  // Decrement Channel
  pElemRef = gslc_ElemCreateBtnTxt(pGui,E_ELEM_BTN_DEC,THIS_PAGE,
    DEC_BUTTON_COORDS,(char*)"-",0,E_FONT_DISPLAY,&CbBtnLora);
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
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GSLC_COL_RED,GSLC_COL_YELLOW);
  gslc_ElemSetRoundEn(pGui,pElemRef,true);
}
