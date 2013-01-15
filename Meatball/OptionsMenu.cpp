#include "OptionsMenu.h"
/*

OptionsMenu::OptionsMenu()
{
	// Init Font ////
	font = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 13, TTF_STYLE_BOLD );
	this->done = 0;
}

OptionsMenu::~OptionsMenu()
{
	if( font ) 
	{
		TTF_CloseFont( font );
	}
}

void OptionsMenu::Update()
{
	// Graphics -> Update

	if ( SDL_GetTicks() > ttDrawCur )
	{
		ttDrawCur = SDL_GetTicks() + 500;
		DrawCur = !DrawCur;
	}
}

void OptionsMenu::Draw()
{
	
}

void OptionsMenu::Main( void )
{
	//pAudio->PlayMusik( MUSIC_DIR "menumusic.mp3");
	

	// Preload the Button Sound
	// SMan->Add( pAudio->LoadSound( SOUNDS_DIR "Button_1.ogg" ), SOUNDS_DIR "Button_1.ogg" );

	// Use this code to Add images and register them as Sprites
	
	//Light1 = new cBasicSprite( IMan->GetPointer( "Light_1" ), Screen->w/7, Screen->h/2 );

	// Resize The Background image
	//Menu_Background->SetSize( (double)Screen->w, (double)Screen->h );
	
	//SDL_Event event;
	
	//done = 0;
	//paused = false;
	
	while( !done )
	{
		pMouse->Update();
		pAudio->Update();
		
		// Call event Handler


		// Update Graphics
		Update();
		//Menu_Background->Update();
		//Light1->Update();
		//Button_Start->Update();
		//Button_Exit->Update();

		// Draw Graphics
		// Fill the Screen with a Backdrop Color first
		SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 60, 0, 0 ) );
		
		//Menu_Background->Draw( Screen ); // Background
		//Light1->Draw( Screen );
		//Button_Start->Draw( Screen );
		//Button_Exit->Draw( Screen );
		
		pMouse->Draw( Screen );
		
		SDL_Flip( Screen );
		
		pFramerate->SetSpeedFactor( );

		if( done == 1 ) // Start
		{

			mode = MODE_MAINMENU;	// Back to MAIN MENU
			
			// Play Sound effects or whatnot...
			//pAudio->PlaySound( SMan->GetPointer( SOUNDS_DIR "Button_1.ogg" ) );
			//pAudio->FadeOutMusic( 2000 );
		}
	}
}

void OptionsMenu::Browse_Handler()
{

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
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					
				}
				else if( event.key.keysym.sym == SDLK_RETURN && ( event.key.keysym.mod & KMOD_ALT ) ) 
				{
					SDL_ToggleFS(Screen);
				} 
				else if( event.key.keysym.sym == SDLK_RETURN )
				{
					
				}
				break;
			}
		case SDL_MOUSEBUTTONUP:
			{
				if( event.button.button == 1 )
				{
					// Check for COllisions Here

					
				}
				break;					
			}
		default:
			break;
		}
	}

	if( MouseCollidesWith( &Button_Exit->rect ) && Button_Exit->GetAlpha() < 245 ) 
	{
		int nAlpha = Button_Exit->GetAlpha() + (int)( 10.0 * pFramerate->speedfactor );

		if( nAlpha > 255 ) 
		{
			nAlpha = 255;
		}

		Button_Exit->SetAlpha( nAlpha );
	}
	else if( Button_Exit->GetAlpha() > 140 && !MouseCollidesWith( &Button_Exit->rect ) )
	{
		int nAlpha = Button_Exit->GetAlpha() - (int)( 5.0 * pFramerate->speedfactor );

		if( nAlpha < 0 ) 
		{
			nAlpha = 0;
		}

		Button_Exit->SetAlpha( nAlpha );
	}

	if( MouseCollidesWith( &Button_Start->rect ) && Button_Start->GetAlpha() < 245 ) 
	{
		int nAlpha = Button_Start->GetAlpha() + (int)( 10.0 * pFramerate->speedfactor );

		if( nAlpha > 255 ) 
		{
			nAlpha = 255;
		}

		Button_Start->SetAlpha( nAlpha );
	}
	else if( Button_Start->GetAlpha() > 140 && !MouseCollidesWith( &Button_Start->rect ) )
	{
		int nAlpha = Button_Start->GetAlpha() - (int)( 5.0 * pFramerate->speedfactor );

		if( nAlpha < 0 ) 
		{
			nAlpha = 0;
		}

		Button_Start->SetAlpha( nAlpha );
	}
	
	if( Light_reverse == true ) 
	{
		double nAlpha = (double)Light1->GetAlpha();
		if( nAlpha < 200 ) 
		{
			nAlpha += ( 10.0 * pFramerate->speedfactor );
			if( nAlpha > 255 ) 
			{
				nAlpha = 255;
				Light_reverse = false;
			}
			Light1->SetAlpha( (int)nAlpha  );

		}
		else
		{
			//printf ("setting to false: %f\n", pFramerate->speedfactor);
			Light_reverse = false;
		}
	}
	else
	{
		double nAlpha = (double)Light1->GetAlpha();
		if( nAlpha > 35 ) 
		{
			
			if( nAlpha > 100 ) 
			{
				nAlpha -= ( 10.0 * pFramerate->speedfactor );
				
			}
			else
			{
				nAlpha -= ( 2.0 * pFramerate->speedfactor );

			}
			
			if (nAlpha < 0) 
			{
				Light_reverse = true;
				nAlpha = 0;
			}
			Light1->SetAlpha( (int)nAlpha  );
			//printf("alpha: %d\n",nAlpha);
			
		}
		else
		{
			//printf ("setting to true %f\n", pFramerate->speedfactor);
			Light_reverse = true;
		}
	}
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
					if ( !constr.empty() )
					{
						constr.erase( constr.end()-1 , constr.end() );
					}
				}	
				else if ( event.key.keysym.sym == SDLK_RETURN )
				{	
					if ( !constr.empty() )
					{
						for ( int i = 9; i >= 0; i-- )
						{
							if ( !strcpy[i].empty() )
							{
								strcpy[i + 1] = strcpy[i];
							}
						}

						strcpy[0] = constr;

						constr.clear(); // Clear console input line
						CMDHandler( strcpy[0] );

						
					}
				}
				else if ( event.key.keysym.sym == SDLK_UP )
				{	
					if (histcounter >= NUM_LINES)
					{	histcounter = NUM_LINES; }
					else constr = strcpy[++histcounter];
					
				}
				else if ( event.key.keysym.sym == SDLK_DOWN )
				{	
					if (histcounter <= 0)
					{	constr.clear(); histcounter = -1;}
					else constr = strcpy[--histcounter];
					
				}
				else
				{
					if( event.key.keysym.unicode )
					{
						constr += (char)event.key.keysym.unicode;
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

*/