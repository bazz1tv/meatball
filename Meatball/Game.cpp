
#include "include\Globals.h"
#include "include\Game.h"
#include "playlist.h"

#define JUMP_KEYS_TRIGGERED (event.key.keysym.sym == (SDLK_LALT) || event.key.keysym.sym == (SDLK_RALT) ||event.key.keysym.sym == SDLK_SEMICOLON || event.key.keysym.sym == SDLK_PERIOD || event.key.keysym.sym == SDLK_m)

// [EHandling Code]
typedef void (*Event_Handlers)(void);
typedef void (*Heldkey_Handlers)(void);
Event_Handlers eventhandlers[NUM_GAMEMODES];
Heldkey_Handlers HeldKeys_Handlers[NUM_GAMEMODES];
// [EHandling Code]

int main( void )
{
	
	InitEP();
	InitSDL( SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE );
	SetWindowCaption( "MeatBall - Vegetable Destruction" );
	SDL_Surface* icon = SDL_LoadBMP("data/favicon.bmp");
	Uint32 colorkey = SDL_MapRGB(icon->format, 128, 128, 128);
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
	SDL_WM_SetIcon(icon, NULL);


	pPreferences	= new cPreferences();
	pGameSettings	= new cSettings();

	pPreferences->Load();
	pPreferences->Apply();

	/* new code */
	screeninfo	= SDL_GetVideoInfo();

	Screen		= InitScreen( pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp, pGameSettings->Fullscreen, MEATBALL_CUSTOM_FLAGS );

	keys		= SDL_GetKeyState( NULL );

	

	pFramerate = new cFramerate( 60 );
	SetSpriteSpeedfactor( &pFramerate->speedfactor );

	IMan	= new cImageManager();
	SMan	= new cSoundManager();
	
	pFont	= new cFont();
	
	pAudio = new cAudio();
	pAudio->bMusic		 = pGameSettings->Music;
	pAudio->bSounds		 = pGameSettings->Sounds;
	pAudio->Sound_Volume = pGameSettings->svol;
	pAudio->Music_Volume = pGameSettings->mvol;
	pAudio->InitAudio();
	
	// Assign the Event Handlers by GAME_MODE
	// [EHandler Code]
	eventhandlers[MODE_MENU]		= &menu_ehandler;
	eventhandlers[MODE_GAME]		= &game_ehandler;
	eventhandlers[MODE_LEVELEDITOR] = &leveleditor_ehandler;
	eventhandlers[MODE_CONSOLE]		= &console_ehandler;
	// [EHandler Code]

	HeldKeys_Handlers[MODE_MENU]			= &menu_heldkeys_handler;
	HeldKeys_Handlers[MODE_GAME]			= &game_heldkeys_handler;
	HeldKeys_Handlers[MODE_CONSOLE]		= &console_heldkeys_handler;
	HeldKeys_Handlers[MODE_LEVELEDITOR]	= &leveleditor_heldkeys_handler;

	mode = MODE_MENU;

	StartGame();

	QuitGame();

	SDL_FreeSurface(icon);
	icon = NULL;

	return 0;
}

/*  */

int MeatBall( void )
{
	Playlist::ls_music();
	done = 0;
	mode = MODE_GAME;

	pPlayer->init();
	pPlayer->Reset();

	Mix_HookMusicFinished(Playlist::musicover); //  Call with NULL to remove the callback.
	pLevel->Load( "test.lvl" );
	string filename = MUSIC_DIR + pLevel->pLevelData->Musicfile;
	pAudio->PlayMusik((char*)filename.c_str(), 1);
	

	
	while( !done )
	{
		if ( !paused )
		{
			Call_Specific_Events_Handler();
			Call_Specific_HeldKeys_Handler();
			Game_Update();
		}
			Game_Draw();
	}

	if( done != 2 )
	{
		return 0;
	}
	else
	{
		return 2;
	}
	
}

/** Calls a LUT array of function pointers to different handlers depending on Game mode */
void Call_Specific_Events_Handler( void )
{

	(*eventhandlers[mode])();
}

/// Calls a LUT array of function pointers to different Handlers depending on GAME_MODE */
void Call_Specific_HeldKeys_Handler( void )
{
	(*HeldKeys_Handlers[mode])();
}

/// Main menu has its own Event handler, so this one is blank.
void menu_ehandler()
{
	// need a blank handler for modes with no events
}

void game_ehandler()
{
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_VIDEORESIZE:
			{
				
				 //Resize the screen
				Screen = SDL_SetVideoMode( event.resize.w, event.resize.h, pGameSettings->Screen_Bpp, MEATBALL_CUSTOM_FLAGS );
				//If there's an error
				if( Screen == NULL )
				{
					printf ("OOPS");
					//windowOK = false;
					return;
				}
				break;
			}
		case SDL_VIDEOEXPOSE:
			{
				//printf ("OOPS");
				//Update the screen
				if( SDL_Flip( Screen ) == -1 )
				{
					//If there's an error
					//windowOK = false;
					printf ("OOPS");
					return;
				}
				break;
			}
		case SDL_QUIT:
			{
				done = 2;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					// universal!
					done = 1;
				}
				else if( event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_COMMA )
				{
					pPlayer->Fire( );
				}
				else if( JUMP_KEYS_TRIGGERED )
				{
					pPlayer->Jump();
				}
				else if( event.key.keysym.sym == SDLK_1 )
				{
					pPlayer->ChangeActiveWeapon( WEAPON_PISTOL );	
				}
				else if( event.key.keysym.sym == SDLK_2 ) 
				{
					pPlayer->ChangeActiveWeapon( WEAPON_MACHINEGUN );	
				}
				else if( event.key.keysym.sym == SDLK_3 ) 
				{
					pPlayer->ChangeActiveWeapon( WEAPON_LASER_X1 );
				}
				else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
				{
					oldmode = mode;
					mode = MODE_CONSOLE;	
				}
				else if( event.key.keysym.sym == SDLK_F8 )
				{
					mode = MODE_LEVELEDITOR;
				}

				else if( event.key.keysym.sym == SDLK_RETURN && ( event.key.keysym.mod & KMOD_ALT ) ) 
				{
					SDL_ToggleFS(Screen);
				}
				
				
				
				else if( event.key.keysym.sym == SDLK_F1 ) // Particle Test
				{
					AddParticleEmitter( pPlayer->posx + ( pPlayer->width/2 ), pPlayer->posy + ( pPlayer->height/2 ) - 10, 9, 255, 250, 200, 0.01, 500, 5 );
				}
				else if( event.key.keysym.sym == SDLK_F2 ) // Enemy Test
				{
					AddEnemy( 200, 0 + (double)( Screen->h/2 ), ENEMY_AF373 );
				}
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					pPlayer->Fire();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{
					pPlayer->Jump();
				}
				break;					
			}
		case SDL_MOUSEBUTTONUP:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{

				}
				break;	
			}
		default:
			break;
		}
	}
}

/** Calls the cConsole::EventHandler().

Since the Console has its own internal Event handler. We call it.
console_ehandler() is the result of a function pointer LUT, therefore
a direct call to EventHandler() cannot be made. This has to do with cConsole::EventHandler()
being a class member, and the other function pointers point to non-class member functions.

To remedy this, we simply call a generic function that will call the member function. That is this function :)
*/
/// @ingroup Console_Input
void console_ehandler(void)
{
	pConsole->EventHandler(); // Nanananan foo foo ;P
}

void menu_heldkeys_handler()
{
}
void game_heldkeys_handler()
{
	if( keys[SDLK_RIGHT] || keys[SDLK_d] ) 
	{
		pPlayer->direction = RIGHT;
	}
	else if( keys[SDLK_LEFT] || keys[SDLK_a] ) 
	{
		pPlayer->direction = LEFT;
	}

	if( keys[SDLK_UP] || keys[SDLK_w] ) 
	{
		pPlayer->updown_pressed = UP;
	}
	else if( keys[SDLK_DOWN] || keys[SDLK_s] ) 
	{
		pPlayer->updown_pressed = DOWN;
	}
	else
	{
		pPlayer->updown_pressed = NONE;
	}
	
	if( keys[SDLK_RCTRL] )
	{
		if( mode == MODE_GAME ) 
		{
			pPlayer->Fire( );
		}
	}
}
void console_heldkeys_handler()
{
}
void leveleditor_heldkeys_handler()
{
	if( keys[SDLK_RIGHT] ) 
	{
		pCamera->Move( 10 * pFramerate->speedfactor, 0 );
	}
	else if( keys[SDLK_LEFT] ) 
	{
		pCamera->Move( -10 * pFramerate->speedfactor, 0 );
	}
	else if( keys[SDLK_UP] ) 
	{
		pCamera->Move( 0, -10 * pFramerate->speedfactor );
	}
	else if( keys[SDLK_DOWN] ) 
	{
		pCamera->Move( 0, 10 * pFramerate->speedfactor );
	}
	
	
	if ( keys[SDLK_e] )
	{
		pLevelEditor->DeleteObject();
	}
	else if (keys[SDLK_RALT])
	{
		


		
	}
}


void Game_Update( void )
{	
	pFramerate->SetSpeedFactor(); // Update
	
	pCamera->Update();
	
	pMouse->Update();
	
	if( mode == MODE_LEVELEDITOR ) 
	{
		pLevelEditor->Update();
	}
	else if ( mode == MODE_CONSOLE )
	{
		pConsole->UpdateConsole();
	}
	
	if( mode == MODE_GAME || mode == MODE_CONSOLE ) 
	{
		pLevel->Update();
		UpdateEnemies();
		UpdateBullets();
		UpdateParticleEmitter();
		UpdatePlayer();
	}
}

void Game_Draw( void )
{
	SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 60, 0, 0 ) );

	pLevel->Draw();

	if( mode != MODE_LEVELEDITOR )
	{
		DrawBullets();
		DrawParticleEmitter();
	}

	pPlayer->Draw( Screen );
	
	DrawEnemies();

	if( mode == MODE_LEVELEDITOR ) 
	{
		pMouse->Draw( Screen );
		pLevelEditor->Draw( Screen );
	}
	else if ( mode == MODE_CONSOLE )
	{
		pConsole->DisplayConsole( Screen );
	}

	FramerateDraw( Screen );
	
	SDL_Flip( Screen );
}


