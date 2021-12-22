#include "packet.h"
#include "../gui/gui.h"

#define MAX_STR 32

static char s_buf[MAX_STR] = {0};

static const char *s_wheele_mode[] =
{
   "AUTO",
   "STOPPED",
   "MANUAL"
};

static const char *s_navstate[] =
{
   "RETREAT FROM CONE",
   "RETREAT",
   "TRACK PATH",
   "TURN TO TARGET",
   "TOUCH TARGET ",
   "SEARCH IN PLACE"
};

void packet_handler(SerialTransfer& transfer){
   gslc_tsElemRef* pGuiElem = NULL;
   gslc_tsGui *pGui = gui_get_ref();

   switch(transfer.currentPacketID()){
      case PACKET_ID_ECHO_TEST:
         // echo
         for(uint16_t i=0; i < transfer.bytesRead; i++)
            transfer.packet.txBuff[i] = transfer.packet.rxBuff[i];

         transfer.sendData(transfer.bytesRead);
         break;

      case PACKET_ID_WHEELE_MODE:
         uint8_t mode;
         transfer.rxObj(mode);

         pGuiElem = gslc_PageFindElemById(pGui,E_PG_CTRL,E_ELEM_CTRL_ID_WHEELE_MODE);
         snprintf(s_buf,MAX_STR,"MODE: %s",s_wheele_mode[mode]);
         gslc_ElemSetTxtStr(pGui,pGuiElem,(const char*)s_buf);
         gslc_ElemSetRedraw(pGui,pGuiElem,GSLC_REDRAW_FULL);
         break;

      case PACKET_ID_NAV_STATE:
         uint8_t nav_state;
         transfer.rxObj(nav_state);
         nav_state += 2; // offset to match wheele nav states

         pGuiElem = gslc_PageFindElemById(pGui,E_PG_CTRL,E_ELEM_CTRL_ID_NAV_STATE);
         snprintf(s_buf,MAX_STR,"STATE: %s",s_navstate[nav_state]);
         gslc_ElemSetTxtStr(pGui,pGuiElem,(const char*)s_buf);
         gslc_ElemSetRedraw(pGui,pGuiElem,GSLC_REDRAW_FULL);
         break;

      default:
         break;
   }


}
