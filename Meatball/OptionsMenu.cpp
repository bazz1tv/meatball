
#include "Globals.h"

//extern MainMenu *mainmenu;
int base_pos, base_adjust;

OptionsMenu::OptionsMenu()
{
	status = STATUS_BROWSING;
	
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


	sMusicVol = new Slider(x+tMusicVol->surface->w+20, tMusicVol->rect.y+18, 256, 0, 128);
	sSoundVol = new Slider(x+tSoundVol->surface->w+20, tSoundVol->rect.y+18, 256, 0, 128);
	sMusicVol->SetAdjusterX(sMusicVol->doInverseCalculation(pPreferences->pSettings->mvol));
	sSoundVol->SetAdjusterX(sSoundVol->doInverseCalculation(pPreferences->pSettings->svol));
}

OptionsMenu::~OptionsMenu()
{
	
	/*SDL_FreeSurface(tMusicVolsurface);
	SDL_FreeSurface(SoundVol.surface);
	SDL_FreeSurface(Exit.surface);*/

	delete sSoundVol;
	delete sMusicVol;

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

	if (status == STATUS_SLIDING_SVOL)
	{
		sSoundVol->Slide((int)floor(pMouse->posx));
		int svol = sSoundVol->doCalculation();
		pAudio->SetAllSoundsVolume(svol);
		//playSoundEffect(svol);
		// that function will play a sound effect only if one isn't playing already
		// look into SDL_Mixer doc
	}

	if (status == STATUS_SLIDING_MVOL)
	{
		sMusicVol->Slide((int)floor(pMouse->posx));
		int mvol = sMusicVol->doCalculation();
		pAudio->SetMusicVolume(mvol);
		// If music is not playing, this section here should throw on a tune
		if (pAudio->MusicPlaying() != true)
			pAudio->PlayMusik(MUSIC_DIR "Stellar.mp3");
	}
	
}


void OptionsMenu::Draw()
{
	SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 00, 0, 0 ) );

	tMusicVol->Draw();
	tExit->Draw();
	tSoundVol->Draw();

	sMusicVol->Draw();
	sSoundVol->Draw();

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
	Uint8 ms = SDL_GetMouseState(NULL,NULL);
	if ((ms & SDL_BUTTON(SDL_BUTTON_LEFT)) && (status == STATUS_SLIDING_SVOL || status == STATUS_SLIDING_MVOL))
	{
		
	}
	switch ( event.type )
	{
		case SDL_KEYDOWN:
		{
			
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == 1)
				{
					if (MouseCollidesWith(&sSoundVol->adjuster_rect) && status == STATUS_BROWSING)
					{
						sSoundVol->Activate();
						printf("DERP");
						status = STATUS_SLIDING_SVOL;
					}
					else if (MouseCollidesWith(&sMusicVol->adjuster_rect) && status == STATUS_BROWSING)
					{
						sMusicVol->Activate();
						printf("DERP");
						status = STATUS_SLIDING_MVOL;
					}
				}
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

				else if (status == STATUS_SLIDING_SVOL)
						status = STATUS_BROWSING;
				else if (status == STATUS_SLIDING_MVOL)
						status = STATUS_BROWSING;
					
				
				
				/// [Mouse Collision Check]
			}
			break;					
		}
		default:
			break;
	}
}
