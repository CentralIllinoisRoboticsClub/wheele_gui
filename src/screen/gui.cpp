#include "../debug/debug.h"
#include "gui.h"

gslc_tsGui     m_gui;
gslc_tsDriver  m_drv;
gslc_tsFont    m_asFont[MAX_FONT];
gslc_tsPage    m_asPage[MAX_PAGE];


void gui_init(void)
{
   /* Setup debug output on Serial */
   gslc_InitDebug(&DebugOut);

   /* Initialize GUISlice */
   gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT);

   /* Fonts */
   gslc_FontAdd(&m_gui,E_FONT_BTN,GSLC_FONTREF_PTR,&FreeMonoBold9pt7b,1);
   gslc_FontAdd(&m_gui,E_FONT_TITLE,GSLC_FONTREF_PTR,&FreeSerifBold9pt7b,1);

   /* Background */
   gslc_SetBkgndColor(&m_gui,GUI_COL_BKGND);

   /* Screens */
   init_base_screen(&m_gui);
   init_home_screen(&m_gui);
   init_setup_screen(&m_gui);
   init_lora_screen(&m_gui);
   init_shutdown_screen(&m_gui);

   gslc_SetPageCur(&m_gui,E_PG_HOME);
}

void gui_update(void)
{
   gslc_Update(&m_gui);
}
