#include "packet.h"
#include "../gui/gui.h"

#define MAX_STR 32

typedef struct {
   float x; float y; float z;
   float yaw; float pitch; float roll;
}cmd_vel_t;

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

      case PACKET_ID_CMD_VEL:
         cmd_vel_t cmd_vel;
         transfer.rxObj(cmd_vel);

         pGuiElem = gslc_PageFindElemById(pGui,E_PG_CTRL,E_ELEM_CTRL_ID_CMD_VEL_LIN);
         snprintf(s_buf,MAX_STR,"CMDVEL(m/s):%.2f, %.2f, %.2f",cmd_vel.x,cmd_vel.y,cmd_vel.z);
         gslc_ElemSetTxtStr(pGui,pGuiElem,(const char*)s_buf);
         gslc_ElemSetRedraw(pGui,pGuiElem,GSLC_REDRAW_FULL);

         pGuiElem = gslc_PageFindElemById(pGui,E_PG_CTRL,E_ELEM_CTRL_ID_CMD_VEL_ANG);
         snprintf(s_buf,MAX_STR,"CMDVEL(r/s):%.2f, %.2f, %.2f",cmd_vel.yaw,cmd_vel.pitch,cmd_vel.roll);
         gslc_ElemSetTxtStr(pGui,pGuiElem,(const char*)s_buf);
         gslc_ElemSetRedraw(pGui,pGuiElem,GSLC_REDRAW_FULL);
         break;

      default:
         break;
   }


}
