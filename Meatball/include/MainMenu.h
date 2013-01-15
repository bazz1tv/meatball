#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "include\Globals.h"
#include "Game.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();


	cBasicSprite *Button_Start,*Button_Exit;
	cBasicSprite *Menu_Background;
	
	cBasicSprite *Light1;
	SDL_Event event;

	int done;
	bool Light_reverse;

	void FadeOutBG();
	void PostDraw();
	void Loop();
	void Update();
	void Collisions();
	void UpdateGraphics();
	void UpdateHardware();
	void UpdateLogic();
	int	DoYouThang();
	void Poll();
};


#endif