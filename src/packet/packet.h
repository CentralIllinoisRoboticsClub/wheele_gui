#include <SerialTransfer.h>

#define PACKET_ID_WHEELE_MODE (10)
#define PACKET_ID_NAV_STATE   (20)
#define PACKET_ID_CUR_POSE    (30)
#define PACKET_ID_TGT_POSE    (40)
#define PACKET_ID_TGT_WYPT    (50)
#define PACKET_ID_TGT_DIST    (60)
#define PACKET_ID_ROS_ALIVE   (70)
#define PACKET_ID_CMD_VEL     (80)
#define PACKET_ID_ECHO_TEST   (90)

void packet_handler(SerialTransfer& transfer);
