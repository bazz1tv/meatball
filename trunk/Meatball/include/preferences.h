
#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "include\Globals.h"

/// Current Settings Revision
#define PREF_VERSION "0.5"

/// User Settings File
#define USER_PREF_NAME "Preferences.ini"
/// Default Settings File
#define DEF_PREF_NAME "default.ini"

class cSettings
{
public:
	cSettings( void );
	~cSettings( void );

	void Apply( void );
	
	void Update( cSettings *Settings );
	
	bool Music;
	bool Sounds;

	Uint8 svol, mvol;

	bool Fullscreen;
	
	Uint16 Screen_W,Screen_H;
	Uint8 Screen_Bpp;

	// Todo
	int Key_look_up,Key_duck,Key_walk_left,Key_walk_right,Key_shoot_primary,Key_shoot_secondary;
	int Key_jump;
};

/// The Preferences
class cPreferences
{
public:
	cPreferences( void );
	~cPreferences( void );
	
	/** Loads the user Settings
	 * if no user Settings file exists loads the
	 * default settings
	 */
	void Load( void );
	/// Loads an given Preferences file
	void Load( string filename );

	/// Saves the Current Settings to the User Settungs File
	void Save( void );

	/// Loads the default settings
	void Default( void );

	/// Sets the in-game settings to the Current Settings
	void Update( void );
	/// Sets the Current Settings to the in-game settings
	void Apply( void );
	
	/// The Settings
	cSettings *pSettings;

private:
	void Parse( char* command, int line );
	int ParseLine( char ** parts, unsigned int count, unsigned int line );
};


#endif