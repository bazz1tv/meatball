#ifndef __MISC_H__
#define __MISC_H__

/* Misc.h

*** Put miscellaneous Routines that may be used by several different files HERE

*/

#include "Globals.h"

void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
void FillRectAlpha(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);



/// Toggle fullScreen
int SDL_ToggleFS(SDL_Surface *surface);

/// More english version of 
bool MouseCollidesWith(SDL_Rect *Crect);

void UniversalEventHandler(SDL_Event*);


struct GraphicsObject
{
	SDL_Surface *surface;
	SDL_Rect rect;
};

#endif