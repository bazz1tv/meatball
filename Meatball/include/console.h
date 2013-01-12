/// @addtogroup Console
// @{
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/// For FileSystem access
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

#include "include\Globals.h"
#include "include\Game.h"

#include "boost.h"

/// Console Commands
bool clearcon( string &str ); // 1
bool loadmap( string &str );
bool SetMx( string &str );
bool SetMy( string &str );
bool SetMxy( string &str );
bool play( string &str );
bool QuitAll( string &str);
bool ShowFPS( string & str);
bool help( string &str );
bool soundVol(string &str);
bool allSoundsVol(string &str);
bool musicVol(string &str); // 12
bool cd(string &str);//13
bool ls(string &str); //14
bool collision(string &str);// 15


/// +1 for the empty handler
#define NUM_CMDS 16
#define NUM_LINES 11

void moveup();
void wait_for_input();
void console_print(char *str);



struct cCMD
{
	bool ( *handler )( string &str );	// Function pointer

	string command;						// string to identify command
	char *helpstr;
	char *syntax;
};

class cConsole
{
public:

	cConsole( void );
	~cConsole( void );
	
	/** Handles console input.
	It's as simple as that */
	/// @ingroup Console_Input
	void EventHandler( void );

	bool CMDHandler( string cInput );
	void UpdateConsole( void );
	
	void DisplayConsole( SDL_Surface *target );

	string ParseBase( string str );
	string ParseParm( string str );

	bool helpCMD( string &str );

	cCMD CMDList[NUM_CMDS];	// The commands

	string strcpy[NUM_LINES];	// history strings (past entered commands which are displayed)

	string constr;		// main string where input is entered into.
	int histcounter;
	fs::path full_path;


	double conx, cony;	// x coord and y coord for str's, x coord for cursor

	bool DrawCur;
	double ttDrawCur;

	cBasicSprite *BG;

	TTF_Font *Console_font;
};



#endif
// @}