/// @addtogroup Game
// @{
#ifndef __GAME_H__
#define __GAME_H__

#include "include\Globals.h"

/// The Game
int MeatBall( void );

/// Depending on which GAME_MODE we are in, the Call_Specific_Events_Handler() function will
/// Call specific Events_Handlers
// @see Event_Handlers
void Call_Specific_Events_Handler( void );

/// Depending on which GAME_MODE we are in, the Call_Specific_HeldKeys_Handler() function will
/// Call specific HeldKeys_Handlers
// @see Event_Handlers
void Call_Specific_HeldKeys_Handler( void );

void Game_Update( void );
void Game_Draw( void );
// @}


/// @addtogroup Event_Handlers
/// The different Event Handlers based on 
/// Each @link GAME_MODE @endlink 
/// @snippet Game.cpp EHandling Code
/// @snippet Game.cpp EHandler Code
// @{
// These are different event handlers depending on game mode
void menu_ehandler(void);
void game_ehandler(void);
// void leveleditor_ehandler(void); // defined in leveleditor.cpp
void console_ehandler(void);
// @}

/// @addtogroup HeldKeys_Handlers
// @{
void menu_heldkeys_handler();
void game_heldkeys_handler();
void console_heldkeys_handler();
void leveleditor_heldkeys_handler();
// @}


#endif
