#include <engine/constants.h>
#include <engine/hgrid.h>
#include <engine/uniform_grid_collision.h>

#ifdef __SH4A__

color_t *vramaddress;
int last_tick;

void init_engine() 
{
    vramaddress = (color_t *) GetVRAMAddress();
    last_tick = RTC_GetTicks();
    init_hgrid();
    init_uniform_grid();
}

#else
bool enableBreakpoints = false;
#endif