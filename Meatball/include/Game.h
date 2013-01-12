/// @addtogroup Game
// @{
#ifndef __MEATBALL_H__
#define __MEATBALL_H__

#include "include\Globals.h"

/// The Game
int MeatBall( void );

void Game_Events( void );
void Game_Input( void );
void Game_Update( void );
void Game_Draw( void );

/// @addtogroup Game_EventHandlers
// These are different event handlers depending on game mode
void menu_ehandler(void);
void game_ehandler(void);
//void leveleditor_ehandler(void); // defined in leveleditor.cpp
void console_ehandler(void);



#endif
// @}