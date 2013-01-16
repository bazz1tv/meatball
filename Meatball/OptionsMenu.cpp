
#include "Globals.h"

//extern MainMenu *mainmenu;

OptionsMenu::OptionsMenu()
{
	
	// Init Font ////
	optionsfont = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 40, TTF_STYLE_BOLD );

	//tExit = TOE->
	MusicVol = TOE->givemeaTextObject();
	SoundVol = TOE->givemeaTextObject();
	Exit	 = TOE->givemeaTextObject();

	MusicVol->writeText("Music Volume", optionsfont);
	SoundVol->writeText("Sound Volume", optionsfont);
	Exit->writeText("Exit", optionsfont);

	int x=20;
	int y=20;

	MusicVol->rect = SetRect( x, y, MusicVol->surface->w, MusicVol->surface->h );
	y+=40;
	SoundVol->rect = SetRect( x, y, SoundVol->surface->w, SoundVol->surface->h );
	y+=80;
	Exit->rect = SetRect( x, y, Exit->surface->w, Exit->surface->h );
	//y+=80;

	MainVol_Slider = new Slider(x+MusicVol->surface->w+20, MusicVol->rect.y, 256, 0, 128);

	SoundVol_Slider = new Slider(x+SoundVol->surface->w+20, SoundVol->rect.y, 256, 0, 128);
}

OptionsMenu::~OptionsMenu()
{
	
	/*SDL_FreeSurface(MusicVolsurface);
	SDL_FreeSurface(SoundVol.surface);
	SDL_FreeSurface(Exit.surface);*/

	delete SoundVol_Slider;
	delete MainVol_Slider;

	if( optionsfont ) 
	{
		TTF_CloseFont( optionsfont );
	}

}

void OptionsMenu::Do()
{
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
		
	//Menu_Background->Draw( Screen ); // Background

	//Light1->Draw( Screen );

	//Button_Start->Draw( Screen );
	//Button_Exit->Draw( Screen );
	
	//SDL_BlitSurface(Options_Text.surface, NULL, Screen, &Options_Text.rect);	
	

	SDL_BlitSurface( MusicVol->surface, NULL, Screen, &MusicVol->rect );
	SDL_BlitSurface( Exit->surface, NULL, Screen, &Exit->rect );
	SDL_BlitSurface( SoundVol->surface, NULL, Screen, &SoundVol->rect );

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
					if( event.button.button == 1 )
					{
						/// [Mouse Collision Check]
						if( MouseCollidesWith( &Exit->rect ) ) 
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
}

