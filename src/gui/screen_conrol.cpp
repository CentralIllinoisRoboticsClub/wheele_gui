#include "gui.h"
#include "elem/XCheckbox.h"

#define THIS_PAGE E_PG_CTRL
#define MAX_STR 32
#define MAX_ELEM  16

static gslc_tsElem    s_Elem[MAX_ELEM];
static gslc_tsElemRef s_ElemRef[MAX_ELEM];
static char s_acTxt[8][MAX_STR] = {0};


void init_control_screen(gslc_tsGui* pGui)
{
  gslc_tsElemRef* pElemRef = NULL;

  gslc_PageAdd(pGui,THIS_PAGE,s_Elem,MAX_ELEM,s_ElemRef,MAX_ELEM);

  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){240,160,40,40},
    (char*)"GPS:",0,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);

  pElemRef = gslc_ElemCreateBox(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){280,170,20,20});
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLACK,GSLC_COL_RED_DK1,GSLC_COL_BLUE);

  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){240,190,40,40},
    (char*)"ROS:",0,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);

  pElemRef = gslc_ElemCreateBox(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){280,200,20,20});
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLACK,GSLC_COL_RED_DK1,GSLC_COL_BLUE);

  // MODE
  snprintf(&s_acTxt[0][0],MAX_STR,"MODE: -");
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){10,40,110,20},
    &s_acTxt[0][0],MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // NAVIGATION STATE
  snprintf(&s_acTxt[1][0],MAX_STR,"NAV STATE: -");
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){10,60,110,20},
    &s_acTxt[1][0],MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // CURRENT HEADING
  snprintf(&s_acTxt[2][0],MAX_STR,"CUR HEAD: -");
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){10,80,110,20},
    &s_acTxt[2][0],MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // TARGET HEADING
  snprintf(&s_acTxt[3][0],MAX_STR,"TRGT HEAD: -");
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){10,100,110,20},
    &s_acTxt[3][0],MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // WAYPOINT TYPE
  snprintf(&s_acTxt[4][0],MAX_STR,"TRGT WYPT: -");
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){10,120,110,20},
    &s_acTxt[4][0],MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);

  // WAYPOINT DISTANCE
  snprintf(&s_acTxt[5][0],MAX_STR,"DIST: -");
  pElemRef = gslc_ElemCreateTxt(pGui,GSLC_ID_AUTO,THIS_PAGE,(gslc_tsRect){10,140,110,20},
    &s_acTxt[5][0],MAX_STR,E_FONT_LIST);
  gslc_ElemSetCol(pGui,pElemRef,GSLC_COL_BLUE,GUI_COL_BKGND,GSLC_COL_BLUE);
  gslc_ElemSetTxtCol(pGui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetTxtAlign(pGui,pElemRef,GSLC_ALIGN_MID_LEFT);
}
