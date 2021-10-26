#include "../debug/debug.h"
#include "gui.h"

gslc_tsGui     *s_pGUI;
gslc_tsDriver  m_drv;
gslc_tsFont    m_asFont[MAX_FONT];
gslc_tsPage    m_asPage[MAX_PAGE];


void gui_init(gslc_tsGui *pGUI)
{
   s_pGUI = pGUI;

   /* Setup debug output on Serial */
   gslc_InitDebug(&DebugOut);

   /* Initialize GUISlice */
   gslc_Init(pGUI,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT);

   /* Fonts */
   gslc_FontAdd(pGUI,E_FONT_BTN,GSLC_FONTREF_PTR,&FreeMonoBold9pt7b,1);
   gslc_FontAdd(pGUI,E_FONT_TITLE,GSLC_FONTREF_PTR,&FreeSerifBold9pt7b,1);
   gslc_FontAdd(pGUI,E_FONT_DISPLAY,GSLC_FONTREF_PTR,&FreeMonoBold9pt7b,2);

   /* Background */
   gslc_SetBkgndColor(pGUI,GUI_COL_BKGND);

   /* Screens */
   init_base_screen(pGUI);
   init_home_screen(pGUI);
   init_setup_screen(pGUI);
   init_lora_screen(pGUI);
   init_shutdown_screen(pGUI);
   init_waypoints_screen(pGUI);

   /* Initial screen */
   gslc_SetPageCur(pGUI,E_PG_HOME);
}

void gui_update(void)
{
   gslc_Update(s_pGUI);
}
