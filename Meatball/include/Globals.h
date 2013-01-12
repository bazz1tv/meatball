#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fstream>

// Include the Engine
#include <SDL_ep.h>

// If we're not WINDOWS, let's change all the sprintf_s calls to regular sprintf
// note: relies on __VA_ARGS__ stackoverflow
#ifdef _WIN32
#define sprintf(buffer, stringbuffer, ...) (sprintf_s(buffer, sizeof(buffer), stringbuffer, __VA_ARGS__)) 
// add for strcpy_s
#define strcpy(dest, src) (strcpy_s(dest, sizeof(dest), src))
// I can ascertain from experience that even if 'dest' argument is something like arg[i++], 
// the i will not get incremented twice from this #define :)
#define strcat(dest,src) (strcat_s(dest, sizeof(dest), src))

#endif
//--------------------------------

#define SDL_CUSTOM_FLAGS (SDL_HWSURFACE | SDL_HWACCEL | SDL_RLEACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE)

// This following line takes out the command line window from your program :) 
#ifndef _DEBUG
	#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif
#pragma comment( lib, "SDL.lib" )
#pragma comment( lib, "SDL_mixer.lib" )
#pragma comment( lib, "SDL_image.lib" )
#pragma comment( lib, "SDL_ttf.lib" )
#pragma comment( lib, "SDL_net.lib" )
#pragma comment( lib, "Sge.lib" )
#pragma comment( lib, "SDL_ep.lib" )

// STL Debugging
#ifndef _DEBUG
#undef _STLP_DEBUG
#else
#define _STLP_DEBUG 1
#endif

// The Directorys
#define PIXMAPS_DIR "data/pixmaps/"
#define MUSIC_DIR "data/music/"
#define SOUNDS_DIR "data/sound/"
#define FONT_DIR "data/font/"
#define LEVEL_DIR "data/levels/"

// Player Directions
#define LEFT	0
#define RIGHT	1
#define UP		2
#define DOWN	3
#define NONE	4

// Modes
#define MODE_MENU 0
#define MODE_GAME 1
#define MODE_LEVELEDITOR 2
#define MODE_CONSOLE 3
#define MODE_DEBUG 4		// Not implemented
#define NUM_GAMEMODES	4

// SDL_ep classes
extern cFramerate *pFramerate;
extern cImageManager *IMan;
extern cSoundManager *SMan;
extern cFont *pFont;
extern cAudio *pAudio;
extern cTCPNet *pTCP;

/// finished with game or not?
extern int done;

/// game paused?
extern bool paused;

// Fonts
extern TTF_Font *Menu_Font;
extern TTF_Font *bold_16;

/// the Screen
extern SDL_Surface *Screen;

/// Keyboard Keystate
extern Uint8 *keys;

/// events
extern SDL_Event event;

/// fps display boolean
extern bool fps_display;

/// Magneta (default) Colorkey
extern Uint32 colorkey;

// Game Global Functions
void StartGame( void );
void QuitGame( void );
//The Main menu
//void Main_Menu( void );
// Declared in MainMenu Module
// 

/// Toggle fullScreen
int SDL_ToggleFS(SDL_Surface *surface);


/// Checks if the file exists
bool FileValid( string filename );
/// Checks if it's an Number
bool is_valid_number( char *c );
/// Draws the Framerate
void FramerateDraw( SDL_Surface *target, double posx = 5.0, double posy = 5.0 );

// includes
#include "include\Preferences.h"
#include "include\Camera.h"
#include "include\Collision.h"
#include "include\sprite.h"
#include "include\effects.h"
#include "include\level.h"
#include "include\weapon.h"
#include "include\player.h"
#include "include\enemy.h"
#include "include\bullet.h"
#include "include\leveleditor.h"
#include "include\console.h"
//Enemies
#include "include\AF-373.h"

// MeatBall
extern cPreferences *pPreferences;	// The Preferences
extern cSettings *pGameSettings;	// The Game Settings
extern cMouseCursor *pMouse;		// The Mouse
extern cCamera *pCamera;			// The Camera
extern cPlayer *pPlayer;			// The Player
extern cLevel *pLevel;				// The Level
extern cLevelEditor *pLevelEditor;	// The Leveleditor
extern cConsole *pConsole;			// The Console

/// screen settings
extern const SDL_VideoInfo* screeninfo;

/// The Current mode
extern Uint8 mode, oldmode;

#endif