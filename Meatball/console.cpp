//#include "include\console.h"
#include "include\Globals.h"
#include <string.h>

using namespace std;

cConsole :: cConsole( void )
{	
	full_path = ( fs::initial_path<fs::path>() );
	histcounter = -1;
	//cout<<full_path.parent_path();
	// Init Background //////////
	if ( pGameSettings->Fullscreen )
	{
		IMan->Add( LoadImage( PIXMAPS_DIR "game/background/conBG.png", colorkey, 140 ), "Console_BG" );
	}
	else // with alpha
	{
		IMan->Add( LoadImage( PIXMAPS_DIR "game/background/conBG.png", colorkey, 140 ), "Console_BG" );
	}

	BG = new cBasicSprite( IMan->GetPointer( "Console_BG" ), 0, 0 );

	BG->SetSize( (double)Screen->w, (double)Screen->h / 3.1);
	/////////////////

	// Init Font ////
	Console_font = pFont->CreateFont( FONT_DIR "NIMBU14.TTF", 14, TTF_STYLE_BOLD );
	////////////////

	CMDList[0].command = "clear";
	CMDList[0].handler = clearcon;
	CMDList[0].helpstr = "Clears all strings in console";
	CMDList[0].syntax  = "clear";

	CMDList[1].command = "loadmap";
	CMDList[1].handler = loadmap;
	CMDList[1].helpstr = "Loads a map file into the game";
	CMDList[1].syntax  = "loadmap [mapfile]";

	CMDList[2].command = "mx";
	CMDList[2].handler = SetMx;
	CMDList[2].helpstr = "Sets Meatball's X coordinate";
	CMDList[2].syntax  = "Mx [x]";

	CMDList[3].command = "my";
	CMDList[3].handler = SetMy;
	CMDList[3].helpstr = "Sets Meatball's Y coordinate";
	CMDList[3].syntax  = "my [y]";

	CMDList[4].command = "mxy";
	CMDList[4].handler = SetMxy;
	CMDList[4].helpstr = "Sets Meatball's X & Y coordinate";
	CMDList[4].syntax  = "Mxy [x y]";

	CMDList[5].command = "play";
	CMDList[5].handler = play;
	CMDList[5].helpstr = "Plays a music file";
	CMDList[5].syntax  = "play [musicfile]";

	CMDList[6].command = "quit";
	CMDList[6].handler = QuitAll;
	CMDList[6].helpstr = "Quits the game";
	CMDList[6].syntax  = "quit";

	CMDList[7].command = "showfps";
	CMDList[7].handler = ShowFPS;
	CMDList[7].helpstr = "Displays or hides FPS";
	CMDList[7].syntax  = "showFPS";

	CMDList[8].command = "help";
	CMDList[8].handler = help;
	CMDList[8].helpstr = NULL;
	CMDList[8].syntax  = NULL;

	CMDList[9].command = "svol";
	CMDList[9].handler = soundVol;
	CMDList[9].helpstr = "Set Sounds Volumes";
	CMDList[9].syntax = "svol [string_id] [0-128]";

	CMDList[10].command = "mvol";
	CMDList[10].handler = musicVol;
	CMDList[10].helpstr = "Set Music Volumes";
	CMDList[10].syntax = "mvol [0-128]";

	CMDList[11].command = "allsvol";
	CMDList[11].handler = allSoundsVol;
	CMDList[11].helpstr = "Set ALL Sounds Channel Volumes";
	CMDList[11].syntax = "allsvol [0-128]";

	CMDList[12].command = "cd";
	CMDList[12].handler = cd;
	CMDList[12].helpstr = "change directory";
	CMDList[12].syntax = "cd [dir]";

	CMDList[13].command = "ls";
	CMDList[13].handler = ls;
	CMDList[13].helpstr = "List Directoy Contents";
	CMDList[13].syntax = "ls [dir]";


	CMDList[NUM_CMDS-1].command.clear();
	CMDList[NUM_CMDS-1].handler = NULL;
	CMDList[NUM_CMDS-1].helpstr = NULL;
	CMDList[NUM_CMDS-1].syntax  = NULL;

	conx = 10.0;
	cony = BG->height - 17;

	DrawCur = false;
	ttDrawCur = SDL_GetTicks() + 500;
}

cConsole :: ~cConsole( void )
{
	if( BG ) 
	{
		delete BG;
	}

	if( Console_font ) 
	{
		TTF_CloseFont( Console_font );
	}

}

void cConsole :: EventHandler( void )
{
	SDL_EnableUNICODE( 1 );
	
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

void cConsole :: UpdateConsole( void )
{
	BG->Update();

	if ( SDL_GetTicks() > ttDrawCur )
	{
		ttDrawCur = SDL_GetTicks() + 500;
		DrawCur = !DrawCur;
	}
}

void cConsole :: DisplayConsole( SDL_Surface *target )
{
	int i;
	// display console BG
	BG->Draw( target );
	
	static double y = 23.0;
	SDL_Rect conput_rect, strcpy_rect[11], cur_rect;
	
	SDL_Surface *conput = NULL;	// input display
	SDL_Surface *sc[11];	// history displays
	SDL_Surface *Cur = NULL;
	
	for( i = 0; i < 11; i++ )
	{
		sc[i] = NULL;
	}
	
	// The blinking Cursor
	if( DrawCur )
	{
		Cur = pFont->CreateText( "_", Console_font );
	}
	else
	{
		Cur = pFont->CreateText( " ", Console_font );
	}

	// Text Creation
	if( !constr.empty() )
	{
		conput = pFont->CreateText( constr.c_str(), Console_font );
	}

	for( i=0; i < 11; i++ )
	{
		if ( !strcpy[i].empty() )
		{
			sc[i] = pFont->CreateText( strcpy[i].c_str(), Console_font );
		}
	}

	// Rect specification

	int curx = 0;
	
	if( conput ) 
	{
		curx += conput->w;
	}
	
	cur_rect = SetRect( (int)(conx + curx), (int)cony-2, Cur->w, Cur->h );

	if( conput )
	{
		conput_rect = SetRect( (int)conx, (int)cony, conput->w, conput->h );
	}

	for( i=0; i < 11; i++ )
	{
		if ( sc[i] )
		{
			strcpy_rect[i] = SetRect( (int)conx, (int)(cony - y), sc[i]->w, sc[i]->h );
			y += 15.0;
		}
	}

	y = 23.0;

	// the actual drawing
	SDL_BlitSurface( Cur, NULL, target, &cur_rect );

	if ( conput )
	{
		SDL_BlitSurface( conput, NULL, target, &conput_rect );
	}

	for ( i=0; i < 11; i++ )
	{
		if ( sc[i] )
		{
			SDL_BlitSurface( sc[i], NULL, target, &strcpy_rect[i] );
		}
	}
	
	if ( Cur )
	{
		SDL_FreeSurface( Cur );
	}

	if ( conput )
	{
		SDL_FreeSurface( conput );
	}

	for ( i=0; i < 11; i++ )
	{
		if ( sc[i] )
		{
			SDL_FreeSurface( sc[i] );
		}
	}
}

bool cConsole :: CMDHandler( string cInput )
{
	string base = ParseBase( cInput );
	string parm = ParseParm( cInput );

	if ( base.empty() )
	{
		cout<<"base empty\n";
		return false;
	}

	cCMD *ptr = CMDList;

	while ( !ptr->command.empty() && ptr->handler )
	{
		if ( base == ptr->command )
		{
			ptr->handler( parm );
			return true;
		}

		ptr++;
	}

	return false;
}

string cConsole :: ParseBase( string str )
{
	size_t found = str.find_first_of( ' ' );
	if (found == string::npos)
		return str;

	string::iterator beginning = str.begin() + found;
	string::iterator ending	= str.end();

	str.erase( beginning, ending );
	cout<<str;
	return str;
}

string cConsole :: ParseParm( string str )
{
	string empty("");
	size_t found = str.find_first_of( ' ' );

	if (found == string::npos)
		return empty;
	string::iterator beginning = str.begin();
	string::iterator ending = str.begin() + found+1;

	str.erase( beginning, ending );
	cout<<str;
	return str;
}

bool cConsole :: helpCMD( string &str )
{
	char buffer[1000];

	//string::iterator pos = str.end() - 1;

	//cout<<(int)*pos;
	//str.erase( pos );
	
	sprintf( buffer, "Revealing information on CMD %s:", str.c_str() );

	int j = 0;

	while ( !CMDList[j].command.empty() )
	{
		if ( CMDList[j].command == str)
		{
			console_print(buffer);
			console_print(CMDList[j].helpstr);
			console_print(CMDList[j].syntax);
			//strcpy[0] = CMDList[j].syntax;
			//strcpy[1] = CMDList[j].helpstr;
			//strcpy[2] = buffer;
			
			return true;
		}

		j++;
	}

	return false;
}






bool clearcon( string &str )
{
	pConsole->constr.clear();

	for ( int i = 0; i < 11; i++ )
	{
		pConsole->strcpy[i].clear();
	}

	pConsole->histcounter = -1;
	return true;
}

bool loadmap( string &str )
{
	pLevel->Load( str );

	return true;
}

bool SetMx( string &str )
{
	pPlayer->posx = atoi( str.c_str() );

	return true;
}

bool SetMy( string &str )
{
	pPlayer->posy = atoi( str.c_str() );

	return true;
}

bool SetMxy( string &str )
{
	size_t found;
	string x,y;
	string::iterator xi, yi;

	// parsing a int,int string combo
	xi = str.begin();

	found = str.find_first_of( ' ' );
	if (found == string::npos)
	{
		return false;
	}
	yi = str.begin() + found;

// Very incompetent String parsing here. The prog will crash if you suck at typing

	x.assign(xi,yi++);
	y.assign(yi, str.end());

	cout << x << " " << y;

	pPlayer->SetPos( atoi( x.c_str() ), atoi( y.c_str() ) );

	return true;
}

bool allSoundsVol(string &str)
{
	int avg = pAudio->SetAllSoundsVolume(atoi(str.c_str()));

	if (avg == -1)
	{
		console_print("Error. Volume invalid");
		return false;
	}
	char result[300];
	sprintf(result,"Successful. Avg/Old volume = %d", avg);
	console_print(result);
	return true;
}

bool soundVol( string &str)
{
	size_t found;
	string sound,vol;
	string::iterator soundi, voli;

	// parsing a int,int string combo
	soundi = str.begin();

	found = str.find_first_of( ' ' );
	if (found == string::npos)
	{
		return false;
	}
	voli = str.begin() + found;

// Very incompetent String parsing here. The prog will crash if you suck at typing

	sound.assign(soundi,voli++);
	vol.assign(voli, str.end());

	cout << sound << " " << vol;

	int oldvol = pAudio->SetSoundVolume(SMan->GetPointer(sound), atoi(vol.c_str()));

	if (oldvol == -1)
	{
		console_print("Error Changing Sound Volume!");
		return false;
	}
	else 
	{
		char Result[200]; // string which will contain the number

		sprintf(Result,"%s_vol : %d => %s", sound.c_str(), oldvol,vol.c_str() ); // %d makes the result be a decimal integer 

		console_print(Result);
	}
	//pAudio->SetSoundsVolume(atoi( str.c_str() ));
	return true;
}

bool musicVol( string &str)
{
	int oldvol = pAudio->SetMusicVolume(atoi( str.c_str() ));
	char result[100];

	if (oldvol == -1)
	{
		console_print("Invalid Volume!");
	}
	else
	{
		sprintf(result,"Music_vol : %d => %s", oldvol, str.c_str());
		console_print(result);
	}
	return true;
}

bool play( string &str )
{
	pLevel->pLevelData->Musicfile = str;

	string file = MUSIC_DIR + str;

	if ( !FileValid( file ) )
	{
		console_print("Error: Music File does not Exist! OR it's already being played ;)");
		return false;
	}
	else
	{
		pAudio->PlayMusik( (char *)str.c_str(), 1,1 );
	}

	return true;
}

bool ShowFPS( string &str )
{
	/*if ( atoi( str.c_str() ) == 1 )
	{
		fps_display = true;
	}
	else if ( atoi( str.c_str() ) == 0 ) 
	{
		fps_display = false;
	}
	else
	{
		pConsole->strcpy[0] = "Invalid Parameter: Only enter 1 or 0..";
		return false;
	}*/

	fps_display = !fps_display;
	
	return true;
}

bool help( string &str )
{
	int j = 0;
	int curline = 0;
	//SDL_Event local_event;
	///bool blah = false;

	if ( str.empty() )
	{		
		/* there are now more than 10 commands... too much for a screen full...
			let's print 10 commands at a time then */
		
		while ( !pConsole->CMDList[j].command.empty() )
		{
			if (curline >= (NUM_LINES))
			{
				
				pConsole->constr = "Press any key..";
				
				wait_for_input();
				
				//curline=0;
				pConsole->constr.clear();
			}
			 
			//moveup();

			console_print((char*)pConsole->CMDList[j++].command.c_str());
			curline++;
			
			//j++;
		}

		
		return true;

	}
	else
	{
		return pConsole->helpCMD( str );
	}
}

void moveup()
{
	int i;
	for (i=NUM_LINES-2; i >= 0; i--)
	{
		pConsole->strcpy[i+1] = pConsole->strcpy[i];
	}
}

bool QuitAll( string &str )
{
	done = 1;

	return true;
}

void wait_for_input()
{
	SDL_Event local_event;
	while (1)
	{
		pConsole->UpdateConsole();
		MeatBall_Draw();
		SDL_PollEvent( &local_event );

		if ( local_event.type == SDL_KEYDOWN )
		{
			break;
		}
	
	}
}

void console_print(char *str)
{
	moveup();
	pConsole->strcpy[0] = str;
}


bool cd(string &str)
{
	if (str.empty())
		return false;
	pConsole->full_path = fs::system_complete(fs::path(pConsole->full_path.string() +"\\"+str));
	console_print((char*)pConsole->full_path.string().c_str());
	return true;
}

bool ls(string &str)
{
	// 1st we won't factor the String parameter
	int curline = 0;
	if ( fs::is_directory( pConsole->full_path ) )
	{
		fs::directory_iterator end_iter;
		char result[500];
		for ( fs::directory_iterator dir_itr( pConsole->full_path ); dir_itr != end_iter; ++dir_itr )
		{
			if (curline >= (NUM_LINES))
			{
				
				pConsole->constr = "Press any key..";
				
				wait_for_input();
				
				//curline=0;
				pConsole->constr.clear();
			}

			string s(dir_itr->path().filename().string());
			try
			{
				if ( fs::is_directory( dir_itr->status() ) )
				{
					sprintf(result,"%s [directory]", s.c_str());
					console_print(result);
				}
				else if ( fs::is_regular_file( dir_itr->status() ) )
				{
					
					console_print((char*)s.c_str());
				}
				else
				{
					sprintf(result,"%s [other]", s.c_str());
					console_print(result);
				}
				curline++;
	
			}
			  catch ( const std::exception & ex )
			  {
				//++err_count;
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			  }
		}
		
	}

	return true;
}