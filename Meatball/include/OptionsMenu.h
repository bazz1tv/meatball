#ifndef __OPTIONSMENU_H__
#define __OPTIONSMENU_H__

#include "Globals.h"

/// @ingroup Options_Screen_Statuses
// @{
#define STATUS_BROWSING 0
#define STATUS_SLIDING	1
#define STATUS_EDITING	2
// @}

struct TextObject
{
	SDL_Surface *surface;
	SDL_Rect rect;
};

class OptionsMenu : public MiniEngine
{
public:
	Uint8 status;
	// StateHandler InputEngine
	
	OptionsMenu();
	~OptionsMenu();

	void Do();
	void Update();
	void Draw();
	void EventHandler();
	//BlinkCursor();
	void LiveInput();
	//Slide(Slider *slider);
	//Keyboard_Joypad_Oneshot();

	//cBasicSprite *Musicvol, *Soundvol; // This is how you store sprites in the class
	//SDL_Surface *cursor;
	//SDL_Event event;
	TTF_Font *optionsfont;

	TextObject Exit, MusicVol, SoundVol;
	
	// inherited from Console (not literally)
	double ttDrawCur;
	bool DrawCur;
	string livestring;
};


#endif