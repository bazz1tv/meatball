//
//  OS.h
//  MeatBall
//
//  Created by bazz on 1/25/13.
//  Copyright (c) 2013 Student. All rights reserved.
//

#ifndef MeatBall_OS_h
#define MeatBall_OS_h

/* OS SPECIFIC DEFINITIONS ---------------------------------- */
/////////////// APPLE ////////////////
#ifdef __APPLE__
//// Since we are using Frameworks on OSX. We must change the include if we are on Windows
#include <MeatBallEngine/SDL_ep.h>

// Call the CTRL key in Windows the META key in Mac
// This needs MORE WORK to be FOOL PROOF
#define SDLK_RCTRL SDLK_RMETA
#define SDLK_LCTRL SDLK_LMETA
#define KMOD_LCTRL KMOD_LMETA
#define KMOD_RCTRL KMOD_RMETA


//

////////////// WIN32 ////////////////
#elif _WIN32
//
// Include the Memory Leak Detection
#ifdef _DEBUG
#include <vld.h>
#endif

#include <SDL_ep.h>

// This takes away the console screen
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

// STL Debugging
#ifndef _DEBUG
#undef _STLP_DEBUG
#else
#define _STLP_DEBUG 1
#endif

// If we're not WINDOWS, let's change all the sprintf_s calls to regular sprintf
// note: relies on __VA_ARGS__ stackoverflow
#define sprintf(buffer, stringbuffer, ...) (sprintf_s(buffer, sizeof(buffer), stringbuffer, __VA_ARGS__))
// add for strcpy_s
#define strcpy(dest, src) (strcpy_s(dest, sizeof(dest), src))
// I can ascertain from experience that even if 'dest' argument is something like arg[i++],
// the i will not get incremented twice from this #define :)
#define strcat(dest,src) (strcat_s(dest, sizeof(dest), src))

#define strdup _strdup
//--------------------------------


#pragma comment( lib, "SDL.lib" )
#pragma comment( lib, "SDL_mixer.lib" )
#pragma comment( lib, "SDL_image.lib" )
#pragma comment( lib, "SDL_ttf.lib" )
#pragma comment( lib, "SDL_net.lib" )
#pragma comment( lib, "Sge.lib" )
#pragma comment( lib, "SDL_ep.lib" )

//
#endif
////////////////////////////////

#endif
