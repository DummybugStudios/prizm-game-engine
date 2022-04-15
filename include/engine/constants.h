#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

#ifdef __SH4A__

#include <fxcg/display.h>
#include <fxcg/rtc.h>

#define HEIGHT LCD_HEIGHT_PX
#define WIDTH LCD_WIDTH_PX

// make it be initialized by init
extern color_t *vramaddress; // initialized by main right now
extern int last_tick; // intialized by main right now

void init_engine();

#else

#define HEIGHT 1080
#define WIDTH 1920
extern bool enableBreakpoints;

#endif

#define OBJECTS 5


#endif