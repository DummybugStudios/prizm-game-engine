#include <engine/constants.h>

#ifdef __SH4A__

color_t *vramaddress;
int last_tick;

void init_engine() 
{
    vramaddress = (color_t *) GetVRAMAddress();
    last_tick = RTC_GetTicks();
}

#else
bool enableBreakpoints = false;
#endif