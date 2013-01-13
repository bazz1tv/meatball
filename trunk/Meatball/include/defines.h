#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SDL_WIN_SETTINGS pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp

// The Directorys
#define PIXMAPS_DIR "data/pixmaps/"
#define MUSIC_DIR "data/music/"
#define SOUNDS_DIR "data/sound/"
#define FONT_DIR "data/font/"
#define LEVEL_DIR "data/levels/"

// Player Directions
/// @addtogroup PLAYER_DIRECTIONS
// @{
#define LEFT	0
#define RIGHT	1
#define UP		2
#define DOWN	3
#define NONE	4
// @}

// Modes
/// @addtogroup GAME_MODE
/// This is a list of the current different types of Game Modes available
// @{
#define MODE_MENU 0
#define MODE_GAME 1
#define MODE_LEVELEDITOR 2
#define MODE_CONSOLE 3
#define MODE_DEBUG 4		///< Not implemented
#define NUM_GAMEMODES	4
// @}

#endif