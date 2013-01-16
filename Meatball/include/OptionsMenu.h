#ifndef __OPTIONSMENU_H__
#define __OPTIONSMENU_H__


class TextObject;
#include "Globals.h"
class Slider;

/// @ingroup Options_Screen_Statuses
// @{
#define STATUS_BROWSING 0
#define STATUS_SLIDING_SVOL	1
#define STATUS_SLIDING_MVOL 2
#define STATUS_EDITING	3
// @}


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
	void Collisions();
	//BlinkCursor();
	void LiveInput();
	//Slide(Slider *slider);
	//Keyboard_Joypad_Oneshot();

	//cBasicSprite *Musicvol, *Soundvol; // This is how you store sprites in the class
	//SDL_Surface *cursor;
	//SDL_Event event;
	TTF_Font *optionsfont;

	TextObject *tExit, *tMusicVol, *tSoundVol;
	
	
	Slider *sMusicVol, *sSoundVol;
	// inherited from Console (not literally)
	double ttDrawCur;
	bool DrawCur;
	string livestring;
};


#endif