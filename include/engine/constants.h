#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdbool.h>

#ifdef __SH4A__

#include <fxcg/display.h>
#define HEIGHT LCD_HEIGHT_PX
#define WIDTH LCD_WIDTH_PX

#else

#define HEIGHT 1080
#define WIDTH 1920

#endif

#define OBJECTS 100

bool enableBreakpoints; // initialized in main.c

#endif