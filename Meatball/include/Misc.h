#ifndef __MISC_H__
#define __MISC_H__

/* Misc.h

*** Put miscellaneous Routines that may be used by several different files HERE

*/

#include "Globals.h"

/// Toggle fullScreen
int SDL_ToggleFS(SDL_Surface *surface);

/// More english version of 
bool MouseCollidesWith(SDL_Rect *Crect);

void UniversalEventHandler(SDL_Event*);
#endif