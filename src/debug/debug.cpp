#include "debug.h"

int16_t DebugOut(char ch)
{
   Serial.write(ch);
   return 0;

}
