#include "gui.h"
#include "../radio/radio.h"

#define THIS_PAGE E_PG_INFO
#define MAX_STR 32
#define MAX_ELEM  15

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 20

#define GUI_VERSION_TXT_COORDS (gslc_tsRect){10,60,BUTTON_WIDTH,BUTTON_HEIGHT}
#define GUI_STRING_TXT_COORDS (gslc_tsRect){10,80,BUTTON_WIDTH,BUTTON_HEIGHT}
#define VBATT_STRING_COORDS (gslc_tsRect){10,40,150,BUTTON_HEIGHT}
#define GSLC_VERSION_STRING_COORDS (gslc_tsRect){10,100,150,BUTTON_HEIGHT}
#define RFM95_VERSION_STRING_COORDS (gslc_tsRect){10,120,150,BUTTON_HEIGHT}

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];
static gslc_tsElemRef *pVbattBox;
static char s_acTxt[MAX_STR] = {0};
static char s_gslice_version[MAX_STR] = {0};
static char s_rfm95_version[MAX_STR] = {0};

void init_info_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);

  // VBATT
  snprintf(s_acTxt,MAX_STR,"GUI BATT:- mV");
  pVbattBox = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    VBATT_STRING_COORDS,s_acTxt,MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pVbattBox,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pVbattBox,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pVbattBox,GSLC_ALIGN_MID_LEFT);

  // WHEELE GUI
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    GUI_VERSION_TXT_COORDS,(char*)"GUI: ",MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    GUI_STRING_TXT_COORDS,(char*)GIT_VERSION,32,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // GUIslice
  snprintf(s_gslice_version,MAX_STR,"GUISlice:%s",gslc_GetVer(pGui));
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    GSLC_VERSION_STRING_COORDS,s_gslice_version,MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // WHEELE TOP


  // WHEELE BOTTOM

  // RFM95
  uint8_t device_version = radio_get_version();
  snprintf(s_rfm95_version,MAX_STR,"LoRa Module: 0x%02X",device_version);
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,
    RFM95_VERSION_STRING_COORDS,s_rfm95_version,MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);
}

void update_info_screen(gslc_tsGui* pGui, uint32_t vbatt)
{
   snprintf(s_acTxt,MAX_STR,"GUI BATT:%dmV\0",vbatt);
   gslc_ElemSetTxtStr(pGui,pVbattBox,(const char*)s_acTxt);
   gslc_ElemSetRedraw(pGui,pVbattBox,GSLC_REDRAW_FULL);
}
