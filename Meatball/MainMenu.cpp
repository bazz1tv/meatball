#include "MainMenu.h"
#include "Game.h"
//#include "Globals.h"

//extern Game game;

int MainMenu::submode = MAIN;

MainMenu::MainMenu()
{
	Light_reverse =SDL_FALSE;
	done = 0;
	submode = MAIN;

	//optionsmenu = OptionsMenu();

	myfont = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 40, TTF_STYLE_BOLD );
	
	//tOptions->rect = SetRect(100,100,tOptions->surface->w, tOptions->surface->h);
	pAudio->PlayMusik( MUSIC_DIR "Monday.it");
	//SMan->Add( pAudio->LoadSound( MUSIC_DIR "menumusic.wav"), MUSIC_DIR "menumusic.wav");
	//pAudio->PlaySound(SMan->GetPointer(MUSIC_DIR "menumusic.wav"));

	// Preload the Button Sound
	SMan->Add( pAudio->LoadSound( SOUNDS_DIR "Button_1.ogg" ), SOUNDS_DIR "Button_1.ogg" );

	/// [Adding a Sprite]
	// Add the image to the Image Manager
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Background3.png", colorkey ), "Menu_Background" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Buttons/Start_32bpp.png", colorkey, 140 ), "Start_Button" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Buttons/Exit_32bpp.png", colorkey, 140 ), "Exit_Button" );
	IMan->Add( LoadImage( PIXMAPS_DIR "Menu/Light1.png", colorkey, 0 ), "Light_1" );
	// Register the sprite
	Menu_Background = new cBasicSprite( Renderer, IMan->GetPointer( "Menu_Background" ), 0, 0 );
	Button_Start = new cBasicSprite( Renderer, IMan->GetPointer( "Start_Button" ), window_width - window_width/3, 250 );
	Button_Exit = new cBasicSprite( Renderer, IMan->GetPointer( "Exit_Button" ), window_width - window_width/3, 250 + (int)Button_Start->height + 125);
	Light1 = new cBasicSprite( Renderer, IMan->GetPointer( "Light_1" ), window_width/7-20, window_height/2 );
	/// [Adding a Sprite]

	// Resize The Background image
	Menu_Background->SetSize( (double)window_width, (double)window_height );

	tOptions = new TextObject(window_width - window_width/3, 250 + (int)Button_Start->height + 50, "Options", myfont);
	SDL_Color optionsColor = SetColor(0,0,0);
	tOptions->SetFGColor(optionsColor);
	tOptions->Render();
	
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
}

MainMenu::~MainMenu()
{
	delete Button_Exit;
	delete Button_Start;

	delete Light1;
	
	delete Menu_Background;

	delete tOptions;

	//SDL_FreeSurface(tOptions.surface);

	if( myfont ) 
	{
		TTF_CloseFont( myfont );
	}

}

void MainMenu::UpdateHardware()
{
	pMouse->Update(Renderer);
	pAudio->Update();
}

void MainMenu::UpdateGraphics()
{
	pFramerate->SetSpeedFactor( );

	Menu_Background->Update(Renderer);
	//Light1->Update();

	Button_Start->Update(Renderer);
	Button_Exit->Update(Renderer);

	tOptions->Update();

	//SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 60, 0, 0 ) );
	
	SDL_RenderClear(Renderer);
	
	Menu_Background->Draw( Renderer ); // Background

	//Light1->Draw( Screen );

	Button_Start->Draw( Renderer );
	Button_Exit->Draw( Renderer );
	tOptions->Draw();
		
	pMouse->Draw( Renderer );
	
	//SDL_RenderPresent(Renderer);
}

/// @todo The Alpha adjustments here could be turned into API calls.
void MainMenu::Collisions()
{
	
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
			int nAlpha = Button_Exit->GetAlpha() - (int)( 10.0 * pFramerate->speedfactor );

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
			int nAlpha = Button_Start->GetAlpha() - (int)( 10.0 * pFramerate->speedfactor );

			if( nAlpha < 0 ) 
			{
				nAlpha = 0;
			}

			Button_Start->SetAlpha( nAlpha );
		}
}

void MainMenu::Update()
{
	UpdateHardware();
	UpdateGraphics();
	Collisions();
	UpdateLogic();
}

void MainMenu::UpdateLogic()
{
	if( Light_reverse ==SDL_TRUE ) 
		{
			double nAlpha = (double)Light1->GetAlpha();
			if( nAlpha < 200 ) 
			{
				nAlpha += ( 10.0 * pFramerate->speedfactor );
				if( nAlpha > 255 ) 
				{
					nAlpha = 255;
					Light_reverse =SDL_FALSE;
				}
				Light1->SetAlpha( (int)nAlpha  );

			}
			else
			{
				//printf ("setting toSDL_FALSE: %f\n", pFramerate->speedfactor);
				Light_reverse =SDL_FALSE;
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
					Light_reverse =SDL_TRUE;
					nAlpha = 0;
				}
				Light1->SetAlpha( (int)nAlpha  );
			}
			else
			{
				Light_reverse =SDL_TRUE;
			}
		}
}



void MainMenu::Do()
{
	//pAudio->PlayMusik( MUSIC_DIR "menumusic.mp3" );

	
	if (submode == MAIN)
	{
		Update();
		EventHandler();
		PostDraw();
	}
	else if (submode == OPTIONS)
	{
		optionsmenu.Do();
	}
	
	// if the mode got switched (we pressed start game or quit)
	if( mode == MODE_GAME ) // Start
	{
#ifndef _DEBUG
		pAudio->PlaySound( SMan->GetPointer( SOUNDS_DIR "Button_1.ogg" ) );
#ifndef DEMO
		pAudio->FadeOutMusic( 2000 );
#endif
		//FadeOutBG();
#endif
		
		Game::Init();
	}
}

void MainMenu::FadeOutBG()
{
	int i; 
	SDL_Surface *surface; 

	surface = SDL_ConvertSurface(Screen, Screen->format, Screen->flags);
	SDL_FillRect( surface, NULL, SDL_MapRGB( Screen->format, 0, 0, 0 ) );
	if(surface == NULL) 
	{ 
		fprintf(stderr, "Load image error: %s\n", 
		SDL_GetError()); exit(-1); 
	} 
	for (i=0; i < 256; i+=2) 
	{ 
		UpdateGraphics();
		//SDL_SetAlpha(surface,SDL_TRUE,(Uint8)i);
		SDL_SetSurfaceAlphaMod(surface, (Uint8)i);
		SDL_BlitSurface(surface,NULL,Screen,NULL); 
		PostDraw();
		//Sleep(10);
	}

}

void MainMenu::PostDraw()
{
	//ScreenTexture = SDL_CreateTextureFromSurface(Renderer, Screen);
	SDL_RenderPresent(Renderer);
	//SDL_Flip( Screen );
}

void MainMenu::EventHandler()
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
					if( event.key.keysym.sym == SDLK_BACKSLASH && (SDL_GetModState() & KMOD_ALT) )
					{
						SDL_ToggleFS(Window);
						
						/*Renderer = GetRenderer(Window, SDL_RENDERER_ACCELERATED); // (Window, -1,
						//Renderer = SDL_GetRenderer(Window);	
						//for (int i=0; i  < 100000000; i++);

						
						Menu_Background->drawimg = SDL_TRUE;
						Button_Start->drawimg = SDL_TRUE;
						Button_Exit->drawimg = SDL_TRUE;
						tOptions->drawtxt = SDL_TRUE;
						pMouse->drawimg = SDL_TRUE;

						optionsmenu.tExit->drawtxt = SDL_TRUE;
						optionsmenu.tMusicVol->drawtxt = SDL_TRUE;
						optionsmenu.tSoundVol->drawtxt = SDL_TRUE;
						optionsmenu.tControls->drawtxt = SDL_TRUE;
						
						optionsmenu.controls_menu.tExit->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tMoveLeft->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tMoveRight->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tFirePrimary->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tJump->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tMoveLeft_CurKey->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tMoveRight_CurKey->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tFirePrimary_CurKey->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tJump_CurKey->drawtxt = SDL_TRUE;
						optionsmenu.controls_menu.tWaitForInput->drawtxt = SDL_TRUE;*/
					}
					if( event.key.keysym.sym == SDLK_ESCAPE )
					{
						mode = MODE_QUIT;
					}
					else if( event.key.keysym.sym == SDLK_RETURN )
					{
						mode = MODE_GAME;
					}
					else if ( event.key.keysym.sym == SDLK_o )
					{
						submode = OPTIONS;
					}
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					if( event.button.button == 1 )
					{
						/// [Mouse Collision Check]
						if( MouseCollidesWith( &Button_Exit->rect ) ) 
						{
							mode = MODE_QUIT; // Exit
						}
						else if( MouseCollidesWith( &Button_Start->rect ) ) 
						{
							mode = MODE_GAME; // Start
						}
						else if( MouseCollidesWith( &tOptions->rect ) )
						{
							submode = OPTIONS;
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
