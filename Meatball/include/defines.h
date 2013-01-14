#ifndef __DEFINES_H__
#define __DEFINES_H__

#define MEATBALL_CUSTOM_FLAGS (SDL_HWSURFACE | SDL_HWACCEL | SDL_RLEACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE)
#define MEATBALL_WIN_SETTINGS pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp


/* The PLAYER */
/// The player won't fall any faster than this (pixels iirc)
#define MAX_FALL_VELOCITY 8
#define JUMP_KEYS_HELD		(keys[pGameSettings->Key_jump]) // || keys[SDLK_RALT] || keys[SDLK_SEMICOLON] || keys[SDLK_PERIOD] || keys[SDLK_m])
#define MOVELEFT_KEYS_HELD	(keys[pGameSettings->Key_walk_left]) //  || keys[SDLK_a])
#define MOVERIGHT_KEYS_HELD (keys[pGameSettings->Key_walk_right]) //|| keys[SDLK_d])
#define JUMP_KEYS_TRIGGERED (event.key.keysym.sym == (pGameSettings->Key_jump)) // || event.key.keysym.sym == (SDLK_RALT) ||event.key.keysym.sym == SDLK_SEMICOLON || event.key.keysym.sym == SDLK_PERIOD || event.key.keysym.sym == SDLK_m)
#define FIRE_KEY_TRIGGERED	(event.key.keysym.sym == (pGameSettings->Key_shoot_primary))
#define FIRE_KEY_HELD	(keys[pGameSettings->Key_shoot_primary])

// The Directorys
#define PIXMAPS_DIR "data/pixmaps/"
#define MUSIC_DIR "data/music/"
#define SOUNDS_DIR "data/sound/"
#define FONT_DIR "data/font/"
#define LEVEL_DIR "data/levels/"

// Player Directions
/// @ingroup PLAYER_DIRECTIONS COLLISION_DIRECTIONS
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