
#ifndef __MEATBALL_H__
#define __MEATBALL_H__

#include "include\Globals.h"

/// The Game
int MeatBall( void );

void MeatBall_Events( void );
void MeatBall_Input( void );
void MeatBall_Update( void );
void MeatBall_Draw( void );

/// These are different event handlers depending on game mode
void menu_ehandler(void);
void game_ehandler(void);
//void leveleditor_ehandler(void); // defined in leveleditor.cpp
void console_ehandler(void);



#endif