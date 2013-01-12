#include "include\MainMenu.h"
#include "include\Game.h"
//#include "include\Globals.h"

void Main_Menu( void )
{

	//printf ("%f\n", pFramerate->speedfactor);
	pAudio->PlayMusik( MUSIC_DIR "menumusic.mp3");

	cBasicSprite *Button_Start,*Button_Exit;
	cBasicSprite *Menu_Background;
	
	cBasicSprite *Light1;

	bool Light_reverse = false;

	// Preload the Button Sound
	SMan->Add( pAudio->LoadSound( SOUNDS_DIR "Button_1.ogg" ), SOUNDS_DIR "Button_1.ogg" );

	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Background.png", colorkey ), "Menu_Background" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Buttons/Start.png", colorkey, 140 ), "Start_Button" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Buttons/Exit.png", colorkey, 140 ), "Exit_Button" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Light1.png", colorkey, 0 ), "Light_1" );

	Menu_Background = new cBasicSprite( IMan->GetPointer( "Menu_Background" ), 0, 0 );
	Button_Start = new cBasicSprite( IMan->GetPointer( "Start_Button" ), Screen->w - Screen->w/3, 250 );
	Button_Exit = new cBasicSprite( IMan->GetPointer( "Exit_Button" ), Screen->w - Screen->w/3, 250 + (int)Button_Start->height + 50);

	Light1 = new cBasicSprite( IMan->GetPointer( "Light_1" ), Screen->w/7, Screen->h/2 );

	// Resize The Background image
	Menu_Background->SetSize( (double)Screen->w, (double)Screen->h );
	
	SDL_Event event;
	
	done = 0;
	paused = false;
	
	while( !done )
	{
		pMouse->Update();
		pAudio->Update();
		
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
						done = 2;
					}
					else if( event.key.keysym.sym == SDLK_RETURN && ( event.key.keysym.mod & KMOD_ALT ) ) 
					{
						SDL_ToggleFS(Screen);
					} 
					else if( event.key.keysym.sym == SDLK_RETURN )
					{
						done = 1;
						
					}
					break;
				}
			case SDL_MOUSEBUTTONUP:
				{
					if( event.button.button == 1 )
					{
						if( pMouse->CollisonCheck( &Button_Exit->rect ) ) 
						{
							done = 2; // Exit
						}
						else if( pMouse->CollisonCheck( &Button_Start->rect ) ) 
						{
							done = 1; // Start
						}
					}
					break;					
				}
			default:
				break;
			}
		}

		if( pMouse->CollisonCheck( &Button_Exit->rect ) && Button_Exit->GetAlpha() < 245 ) 
		{
			int nAlpha = Button_Exit->GetAlpha() + (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha > 255 ) 
			{
				nAlpha = 255;
			}

			Button_Exit->SetAlpha( nAlpha );
		}
		else if( Button_Exit->GetAlpha() > 140 && !pMouse->CollisonCheck( &Button_Exit->rect ) )
		{
			int nAlpha = Button_Exit->GetAlpha() - (int)( 5.0 * pFramerate->speedfactor );

			if( nAlpha < 0 ) 
			{
				nAlpha = 0;
			}

			Button_Exit->SetAlpha( nAlpha );
		}

		if( pMouse->CollisonCheck( &Button_Start->rect ) && Button_Start->GetAlpha() < 245 ) 
		{
			int nAlpha = Button_Start->GetAlpha() + (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha > 255 ) 
			{
				nAlpha = 255;
			}

			Button_Start->SetAlpha( nAlpha );
		}
		else if( Button_Start->GetAlpha() > 140 && !pMouse->CollisonCheck( &Button_Start->rect ) )
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


		Menu_Background->Update();

		Light1->Update();

		Button_Start->Update();
		Button_Exit->Update();

		SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 60, 0, 0 ) );
		
		Menu_Background->Draw( Screen ); // Background

		Light1->Draw( Screen );

		Button_Start->Draw( Screen );
		Button_Exit->Draw( Screen );
		
		pMouse->Draw( Screen );
		
		SDL_Flip( Screen );
		
		pFramerate->SetSpeedFactor( );

		if( done == 1 ) // Start
		{

			mode = MODE_GAME;
			pAudio->PlaySound( SMan->GetPointer( SOUNDS_DIR "Button_1.ogg" ) );
			
			pAudio->FadeOutMusic( 2000 );

			done = MeatBall(); // The Game

			pAudio->PlayMusik( MUSIC_DIR "main_menu.mod" );
		}
	}

	delete Button_Exit;
	delete Button_Start;

	delete Light1;
	
	delete Menu_Background;
}
