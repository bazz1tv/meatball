
#include "include\Globals.h"
#include "include\Game.h"
#include "playlist.h"


typedef void (*EventHandlers)(void);
EventHandlers eventhandlers[NUM_GAMEMODES];

int main( void )
{
	
	InitEP();
	
	InitSDL( SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE );
	
	pPreferences = new cPreferences();
	pGameSettings = new cSettings();

	pPreferences->Load();
	pPreferences->Apply();

	/* new code */
	screeninfo = SDL_GetVideoInfo();

	Screen = InitScreen( pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp, pGameSettings->Fullscreen, SDL_HWSURFACE | SDL_HWACCEL | SDL_RLEACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE );

	keys = SDL_GetKeyState( NULL );
	SetWindowCaption( "MeatBall - Vegetable Destruction" );

	pFramerate = new cFramerate( 60 );
	SetSpriteSpeedfactor( &pFramerate->speedfactor );

	IMan = new cImageManager();
	SMan = new cSoundManager();
	
	pFont = new cFont();
	
	pAudio = new cAudio();
	pAudio->bMusic = pGameSettings->Music;
	pAudio->bSounds = pGameSettings->Sounds;
	pAudio->Sound_Volume = pGameSettings->svol;
	pAudio->Music_Volume = pGameSettings->mvol;
	pAudio->InitAudio();
	

	eventhandlers[MODE_MENU] = &menu_ehandler;
	eventhandlers[MODE_GAME] = &game_ehandler;
	eventhandlers[MODE_LEVELEDITOR] = &leveleditor_eventhandler;
	eventhandlers[MODE_CONSOLE] = &console_ehandler;
	
	mode = MODE_MENU;

	StartGame();

	QuitGame();

	return 0;
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

int MeatBall( void )
{
	Playlist::ls_music();
	done = 0;
	mode = MODE_GAME;

	pPlayer->init();
	pPlayer->Reset();

	pLevel->Load( "test.lvl" );
	string filename = MUSIC_DIR + pLevel->pLevelData->Musicfile;
	pAudio->PlayMusik((char*)filename.c_str(), 1);
	Mix_HookMusicFinished(Playlist::musicover); //  Call with NULL to remove the callback.

	
	while( !done )
	{
		if ( !paused )
		{
			Game_Events();
			Game_Input();
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
void Game_Events( void )
{

	(*eventhandlers[mode])();
}

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
				//double monitorAspectRatio = screeninfo->current_w / screeninfo->current_h;//screenHeight;
				//double imageAspectRatio = 800.0 / 600.0;
				//
				/*if(monitorAspectRatio > imageAspectRatio) {
					 // Here monitor is wider than the image, so the height of the target rectangle should be the screen height, but not stretching all the way to the left/right
					top = 0;
					height = screenHeight;
					width = height * imageAspectRatio;
					left = screenWidth/2 - width/2;
				}
				else {
				// The opposite, left/width is the whole screen, but you don't stretch all the way up/down
					...
				}*/
				 //Resize the screen
				Screen = SDL_SetVideoMode( event.resize.w, event.resize.h, pGameSettings->Screen_Bpp, SDL_CUSTOM_FLAGS );
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
					//game mode code
					
					pPlayer->Fire( );
					
				}
				else if( event.key.keysym.sym == (SDLK_LALT) || event.key.keysym.sym == (SDLK_RALT) ||event.key.keysym.sym == SDLK_SEMICOLON || event.key.keysym.sym == SDLK_PERIOD || event.key.keysym.sym == SDLK_m) 
				{
					//game mode code
					
					pPlayer->Jump();
					
				}
				else if( event.key.keysym.sym == SDLK_1 )
				{
					//game mode code
					
					pPlayer->ChangeActiveWeapon( WEAPON_PISTOL );
					
				}
				else if( event.key.keysym.sym == SDLK_2 ) 
				{
					//game mode code
					
					pPlayer->ChangeActiveWeapon( WEAPON_MACHINEGUN );
					
				}
				else if( event.key.keysym.sym == SDLK_3 ) 
				{
					//game mode code
					
					pPlayer->ChangeActiveWeapon( WEAPON_LASER_X1 );
					
				}
				else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
				{
					oldmode = mode;
					mode = MODE_CONSOLE;
						
				}
				else if( event.key.keysym.sym == SDLK_F8 )
				{
					// game mode code
					mode = MODE_LEVELEDITOR;
				}

				else if( event.key.keysym.sym == SDLK_RETURN && ( event.key.keysym.mod & KMOD_ALT ) ) 
				{
					SDL_ToggleFS(Screen);
				}
				
				
				
				else if( event.key.keysym.sym == SDLK_F1 ) // Particle Test
				{
					//game mode
					AddParticleEmitter( pPlayer->posx + ( pPlayer->width/2 ), pPlayer->posy + ( pPlayer->height/2 ) - 10, 9, 255, 250, 200, 0.01, 500, 5 );
				}
				else if( event.key.keysym.sym == SDLK_F2 ) // Enemy Test
				{
					//game mode
					AddEnemy( 200, 0 + (double)( Screen->h/2 ), ENEMY_AF373 );
				}
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					//level editor mode
						//pLevelEditor->SetMoveObject();
					pPlayer->Fire();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					//level editor mode
					//pLevelEditor->SetFastCopyObject();
					
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{
					//level editor mode
					//pLevelEditor->DeleteObject();
					pPlayer->Jump();
				}
				break;					
			}
		case SDL_MOUSEBUTTONUP:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					//level editor mode
					//pLevelEditor->Release_Command();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					//level editor mode
						//pLevelEditor->Release_Command();
					//level editor mode
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



void console_ehandler(void)
{
	pConsole->EventHandler(); // Nanananan foo foo ;P
}

static int specialpastecounter=0;
void Game_Input( void )
{
	if( mode == MODE_GAME ) 
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
	else if( mode == MODE_LEVELEDITOR ) 
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
			if( keys[SDLK_d]) 
			{
				if (specialpastecounter++ == 0)
				{
					pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x + pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
					pCamera->Move( pLevelEditor->CopyObject->width, 0 );
				}
			}
			else if( keys[SDLK_w] )
			{
				if (specialpastecounter++ == 0)
				{
					pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x, pLevelEditor->CopyObject->posy - pCamera->y - pLevelEditor->CopyObject->height );
					pCamera->Move( 0, - pLevelEditor->CopyObject->height );
				}
			}
			else if( keys[SDLK_a] )
			{
				if (specialpastecounter++ == 0)
				{
					pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x - pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
					pCamera->Move( - pLevelEditor->CopyObject->width, 0 );
				}
			}
			else if( keys[SDLK_s] )
			{
				if (specialpastecounter++ == 0)
				{
					pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx- pCamera->x , pLevelEditor->CopyObject->posy - pCamera->y + pLevelEditor->CopyObject->height );
					pCamera->Move( 0, pLevelEditor->CopyObject->height );
				}
			}

			if (specialpastecounter == 40)
					specialpastecounter = 0;


			
		}
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


