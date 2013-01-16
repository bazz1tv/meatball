
#include "Globals.h"

//extern MainMenu *mainmenu;

OptionsMenu::OptionsMenu()
{
	
	// Init Font ////
	optionsfont = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 40, TTF_STYLE_BOLD );

	int x=20;
	int y=20;

	tMusicVol   = new TextObject(x,y,"Music Volume", optionsfont);
	tSoundVol   = new TextObject(x,y+40,"Sound Volume", optionsfont);
	tExit		= new TextObject(x,y+120,"Exit", optionsfont);

	tMusicVol->setFGColor(SetColor(255,0,0));

	//Render
	tMusicVol->Render();
	tSoundVol->Render();
	tExit->Render();


	MainVol_Slider = new Slider(x+tMusicVol->surface->w+20, tMusicVol->rect.y, 256, 0, 128);

	SoundVol_Slider = new Slider(x+tSoundVol->surface->w+20, tSoundVol->rect.y, 256, 0, 128);
}

OptionsMenu::~OptionsMenu()
{
	
	/*SDL_FreeSurface(tMusicVolsurface);
	SDL_FreeSurface(SoundVol.surface);
	SDL_FreeSurface(Exit.surface);*/

	delete SoundVol_Slider;
	delete MainVol_Slider;

	delete tMusicVol;
	delete tSoundVol;
	delete tExit;

	if( optionsfont ) 
	{
		TTF_CloseFont( optionsfont );
	}

}

void OptionsMenu::Do()
{
	Collisions();
	EventHandler();
	Update();
	Draw();
}

void OptionsMenu::Update()
{
	// Graphics -> Update

	/*if ( SDL_GetTicks() > ttDrawCur )
	{
		ttDrawCur = SDL_GetTicks() + 500;
		DrawCur = !DrawCur;
	}*/


	pMouse->Update();
	pAudio->Update();

	pFramerate->SetSpeedFactor( );

	
}


void OptionsMenu::Draw()
{
	SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 00, 0, 0 ) );

	tMusicVol->Draw();
	tExit->Draw();
	tSoundVol->Draw();

	MainVol_Slider->Draw();
	SoundVol_Slider->Draw();

	pMouse->Draw( Screen );

	PostDraw();
}


void OptionsMenu::LiveInput( void )
{
	SDL_EnableUNICODE( 1 );				// http://sdl.beuc.net/sdl.wiki/SDL_EnableUNICODE
	
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_QUIT:
			{
				done = 2;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					//done = 1;
					mode = oldmode;
				}

				else if ( event.key.keysym.sym == SDLK_BACKSPACE )
				{
					if ( !livestring.empty() )
					{
						livestring.erase( livestring.end()-1 , livestring.end() );
					}
				}	
				else if ( event.key.keysym.sym == SDLK_RETURN )
				{	
					// ACCEPT THE INPUT
				}
				else if ( event.key.keysym.sym == SDLK_UP )
				{	
				}
				else if ( event.key.keysym.sym == SDLK_DOWN )
				{	
				}
				else
				{
					if( event.key.keysym.unicode )
					{
						livestring += (char)event.key.keysym.unicode;
					}
				}
				
				break;
			}
		default:
			break;
		}
	}
	
	SDL_EnableUNICODE( 0 );
}

void OptionsMenu::EventHandler()
{		
		while ( SDL_PollEvent( &event ) )
		{
			UniversalEventHandler(&event);
			switch ( event.type )
			{
				case SDL_QUIT:
				{
					done = MODE_QUIT;
					break;
				}
				case SDL_KEYDOWN:
				{
					if( event.key.keysym.sym == SDLK_ESCAPE )
					{
						mode = MODE_QUIT;
					}
					else if( event.key.keysym.sym == SDLK_RETURN && ( event.key.keysym.mod & KMOD_ALT ) ) 
					{
						SDL_ToggleFS(Screen);
					} 
					else if( event.key.keysym.sym == SDLK_RETURN )
					{
						mode = MODE_GAME;
					}
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					break;					
				}
				default:
					break;
			}
		}
}

void OptionsMenu::Collisions()
{
	switch ( event.type )
	{
		case SDL_KEYDOWN:
		{
			
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if( event.button.button == 1 )
			{
				/// [Mouse Collision Check]
				if( MouseCollidesWith( &tExit->rect ) ) 
				{
					MainMenu::submode = MAIN; // Exit
				}
				
				/// [Mouse Collision Check]
			}
			break;					
		}
		default:
			break;
	}
}
