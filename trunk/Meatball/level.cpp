
#include "Globals.h"


using namespace std;

cLevelData :: cLevelData( void ) :
BasicSprites(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS)
{
	//BasicSprites = NULL;
	//BasicSprites.objcount = 0;
	


	BG_red = 0;
	BG_green = 0;
	BG_blue = 0;

	Musicfile = "badmofo.mod";
}

cLevelData :: ~cLevelData( void )
{
	/*unsigned int i = 0;
	
	if( BasicSprites.objects )
	{
		for( i = 0;i < BasicSprites.objcount;i++ ) 
		{
			if( BasicSprites.objects[i] ) 
			{
				delete BasicSprites.objects[i];
				BasicSprites.objects[i] = NULL; 
			} 
		}

		//delete []BasicSprites;
		SDL_free(BasicSprites);
		BasicSprites = NULL;	
	}
	
	BasicSprites.objcount = 0;	*/
	
	BasicSprites.~ObjectManager();
}

void cLevelData :: AddSprite( cMVelSprite *Sprite )
{
	//BasicSprites = (cMVelSprite**) SDL_realloc( BasicSprites, ++BasicSprites.objcount * sizeof(cMVelSprite*) );
	//BasicSprites[BasicSprites.objcount - 1] = Sprite;
	//return;
	BasicSprites.add(Sprite);
}

int cLevelData :: GetCollidingSpriteNum( SDL_Rect *Crect )
{
	if( BasicSprites.objects && Crect )
	{
		for( unsigned int i = 0; i < BasicSprites.objcount; i++ )
		{
			if( !BasicSprites.objects[i] ) 
			{
				continue;
			}
            
			if( RectIntersect( &(const SDL_Rect&)BasicSprites.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				return i;
			}
		}
	}

	return -1;
}

// This function takes external pointers
SDL_bool cLevelData :: GetAllCollidingSpriteNum( SDL_Rect *Crect, ObjectManager<cMVelSprite> *obj_man )
{
	SDL_bool did_we_get_objects = SDL_FALSE;
	
	if( BasicSprites.objects && Crect )
	{
		//DEBUGLOG( "Using Crect coords x: %d, y: %d, width: %d, height: %d\n", Crect->x,Crect->y, Crect->w, Crect->h);
		
		for( unsigned int i = 0; i < BasicSprites.objcount; i++ )
		{
			if( !BasicSprites.objects[i] )
			{
				continue;
			}
            
			if( RectIntersect( &(const SDL_Rect&)BasicSprites.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				// Add this sprite num to the list
				if (obj_man->hasa(BasicSprites.objects[i]) < 0)
					obj_man->add(BasicSprites.objects[i]);
				
				did_we_get_objects = SDL_TRUE;
			}
		}
	}
	
	return did_we_get_objects;
}

cBasicSprite *cLevelData :: GetCollidingSprite( SDL_Rect *Crect )
{
	if( BasicSprites.objects && Crect )
	{
		for( unsigned int i = 0; i < BasicSprites.objcount; i++ )
		{
			if( !BasicSprites.objects[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)BasicSprites.objects[i]->GetRect( SDL_FALSE ), Crect ) )
			{
				return BasicSprites.objects[i];
			}
		}
	}

	return NULL;
}

void cLevelData :: DeleteSprite( unsigned int number )
{
	// WHY???
	//if( (BasicSprites && number) < (EnemyCount && BasicSprites[number]) ) 
	//{
		cout<<"deleted "<<number;
		delete BasicSprites.objects[number];
		BasicSprites.objects[number] = NULL;
	//}
}

cLevel :: cLevel( void )
{
	levelfile = "none";
	pLevelData_Layer1 = NULL;
}

cLevel :: ~cLevel( void )
{
	Unload();
}

void cLevel :: Load( string filename )
{
	string full_filename = LEVEL_DIR + filename;

	if( !FileValid( full_filename ) )
	{
		DEBUGLOG( "Error : Level does not exist : %s\n", full_filename.c_str() );
		return;
	}

	Unload();
	
	DeleteAllParticleEmitter();
	DeleteAllBullets();
	DeleteAllEnemies();
	
	levelfile = filename;

	pLevelData_Layer1 = new cLevelData();

	ifstream ifs;
	ifs.open( full_filename.c_str(), ios :: in );

	char *contents = new char[500]; // maximal length of an line
	
	for( unsigned int i = 0; ifs.getline( contents, 500 ); i++ )
	{
		Parse( contents, i );
	}

	ifs.close();

	delete contents;
	
	pFramerate->Reset();
}

void cLevel :: Unload( void )
{
	if( pLevelData_Layer1 ) 
	{
		delete pLevelData_Layer1;
		pLevelData_Layer1 = NULL;
	}

	levelfile.clear();
}

void cLevel :: Save( void )
{
	string filename = LEVEL_DIR;
	filename += levelfile.c_str();

	if( !FileValid( filename ) ) 
	{
		DEBUGLOG( "Error Level Save : Level file does not exist %s\n", filename.c_str() );
	}

	ofstream out( filename.c_str(), ios::out );
	ofs = &out;

	if( !ofs )
	{
		DEBUGLOG( "Error Level Save: could not open the Level file %s\n", filename.c_str() );
		return;
	}
	
	PrintSaveHeader();
	SavePlayerPos();
	SaveMusicFile();
	SaveMapObjects();
	SaveMapEnemies();

	DEBUGLOG( "Level saved to %s\n", levelfile.c_str() );
	
	ofs = NULL;
}

void cLevel :: PrintSaveHeader()
{
	sprintf( row,  "### Level Saved with MeatBall FX V.0 ###\n\n" );
	ofs->write( row, strlen( row ) );
	
	sprintf( row,  "### Level Saved on %s\n", Get_Curr_Date().c_str() );
	ofs->write( row, strlen( row ) );
}

void cLevel :: SavePlayerPos()
{
	sprintf( row, "Player %d %d\n", (int) pPlayer->Startposx, (int)window_height - (int)pPlayer->Startposy );
	ofs->write( row, strlen( row ) );
}

void cLevel :: SaveMusicFile()
{
	sprintf( row,  "Music %s\n\n", pLevelData_Layer1->Musicfile.c_str() );
	ofs->write( row, strlen( row ) );
}

void cLevel :: SaveMapObjects()
{
	sprintf( row, "### Map Objects ###\n" );
	ofs->write( row, strlen( row ) );
	
	// Map Objects
	if( pLevelData_Layer1 )
	{
		for( i = 0; pLevelData_Layer1->BasicSprites.objcount > i; i++ )
		{
			if( !pLevelData_Layer1->BasicSprites.objects[i] )
			{
				continue;
			}
			
			string image_filename = IMan->GetIdentifier( pLevelData_Layer1->BasicSprites.objects[i]->srcimage );
			
			image_filename.erase( 0, strlen( PIXMAPS_DIR ) );
			
			if( pLevelData_Layer1->BasicSprites.objects[i]->type == SPRITE_TYPE_MASSIVE )
			{
				sprintf(row,  "Sprite %s %d %d MASSIVE\n", image_filename.c_str(), (int) pLevelData_Layer1->BasicSprites.objects[i]->posx, (int)window_height - (int)pLevelData_Layer1->BasicSprites.objects[i]->posy );
			}
			else if( pLevelData_Layer1->BasicSprites.objects[i]->type == SPRITE_TYPE_PASSIVE )
			{
				sprintf(row,  "Sprite %s %d %d PASSIVE\n", image_filename.c_str(), (int) pLevelData_Layer1->BasicSprites.objects[i]->posx, (int)window_height - (int)pLevelData_Layer1->BasicSprites.objects[i]->posy );
			}
			else if( pLevelData_Layer1->BasicSprites.objects[i]->type == SPRITE_TYPE_HALFMASSIVE )
			{
				sprintf(row,  "Sprite %s %d %d HALFMASSIVE\n", image_filename.c_str(), (int) pLevelData_Layer1->BasicSprites.objects[i]->posx, (int)window_height - (int)pLevelData_Layer1->BasicSprites.objects[i]->posy );
			}
			else
			{
				DEBUGLOG( "Warning Level Saving : Unknown Map Object type : %d\n",pLevelData_Layer1->BasicSprites.objects[i]->type );
				continue;
			}
			
			ofs->write( row, strlen( row ) );
		}
	}
}

void cLevel :: SaveMapEnemies()
{
	sprintf( row, "### Enemies ###\n" );
	ofs->write( row, strlen( row ) );
	
	// Enemies
	if( Enemies.objects )
	{
		for( i = 0; Enemies.objcount > i; i++ )
		{
			if( !Enemies.objects[i] )
			{
				continue;
			}
			
			if( Enemies.objects[i]->Enemy_type == ENEMY_AF373 )
			{
				sprintf( row,  "Enemy %d %d %d\n", (int)Enemies.objects[i]->Startposx, (int)Enemies.objects[i]->Startposy, Enemies.objects[i]->Enemy_type );
				ofs->write( row, strlen( row ) );
			}
			else
			{
				DEBUGLOG( "Level Saving : Wrong Enemy type : %d\n", Enemies.objects[i]->Enemy_type );
			}
		}
	}
}

void cLevel :: Update( void )
{
	if( !Mix_PlayingMusic() )
	{
		string filename = MUSIC_DIR + pLevelData_Layer1->Musicfile;

#ifndef DEMO
		pAudio->PlayMusik( (char *)filename.c_str(),0,1 );
#endif
		
	}
}

void cLevel :: Draw( void )
{
	// For all tiles Update and Draw... Hm don'chu think we should make a seperate Update & Draw Routine ???

	// I'll tell you why we do it all at once... So we only Loop once>!! It's costly to do it twice...
	// it doesn't seem right.

	/* We should be drawing like 5 layers at a time... So hmmmmmm.... .What does that mean??? 

	This seems to be very dependent on the level at the time it goes aT .

	So i Suppose insteadd of... Having.. ... ..... >  I HAVEN O F >> >> >>> 

	 */ 



	for( unsigned int i = 0; i < pLevelData_Layer1->BasicSprites.objcount;i++ )
	{
		if( !pLevelData_Layer1->BasicSprites.objects[i] ) 
		{
			continue;
		}

		pLevelData_Layer1->BasicSprites.objects[i]->Update();
		pLevelData_Layer1->BasicSprites.objects[i]->Draw( Renderer );
	}
}

void cLevel :: Parse( char* command, int line )
{
	if ( strlen( command ) <= 5 || *command == '#')
	{
		return;
	}
	
	char* str = command;
	int count = 0;
	int i = 0;
	
	str += strspn(str," ");
	
	while (*str)
	{
		count++;
		str += strcspn(str," ");
		str += strspn(str," ");
	}
	
	str = command; // reset
	
	char** parts = new char*[ count ];
	
	str += strspn(str," ");
	
	while(*str)
	{
		size_t len = strcspn( str," " );
		parts[i] = (char*)SDL_malloc( len + 1 );
		memcpy( parts[i], str, len );
		parts[i][len] = 0;
		str += len + strspn( str + len, " " );
        
        if (parts[i][len-1] == '\r')
            parts[i][len-1] = 0;
        
		i++;
        
        
	}
	
	// Level Message handler
	ParseLine( parts, count, line );
	
	for( i = 0; i < count ; i++ )
	{
		delete( parts[i] );
	}
	
	delete []parts;
}

int cLevel :: ParseLine( char ** parts, unsigned int count, unsigned int line )
{
	if ( strcmp( parts[0], "Sprite") == 0 )
	{
		if( count != 5 )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG(" Sprite needs 5 parameters\n");
			return 0; // error
		}

		string full_filename = PIXMAPS_DIR;
		full_filename += parts[1];

		if( !FileValid( full_filename ) )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG(" Sprite image does not exist\n");
			return 0; // error
		}

		if( !is_valid_number( parts[2] ) )
		{
			DEBUGLOG( "%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		if( !is_valid_number( parts[3] ) )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}

		IMan->Add( LoadImage( full_filename.c_str(), colorkey ), full_filename );

		if( strcmp( parts[4], "MASSIVE" ) == 0 )
		{
			cMVelSprite *temp = new cMVelSprite( IMan->GetPointer( full_filename ), (double)atoi( parts[2] ), (double)window_height -(double)atoi( parts[3] ) );
			temp->type = SPRITE_TYPE_MASSIVE;
			pLevelData_Layer1->AddSprite( temp );
		}
		else if( strcmp( parts[4], "PASSIVE" ) == 0 )
		{
			cMVelSprite *temp = new cMVelSprite( IMan->GetPointer( full_filename ), (double)atoi( parts[2] ), (double)window_height -(double)atoi( parts[3] ) );
			temp->type = SPRITE_TYPE_PASSIVE;
			pLevelData_Layer1->AddSprite( temp );
		}
		else if( strcmp( parts[4], "HALFMASSIVE" ) == 0 )
		{
			cMVelSprite *temp = new cMVelSprite( IMan->GetPointer( full_filename ), (double)atoi( parts[2] ), (double)window_height - (double)atoi( parts[3] ) );
			temp->type = SPRITE_TYPE_HALFMASSIVE;
			pLevelData_Layer1->AddSprite( temp );
		}
		else
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid Sprite type\n", parts[4] );
			return 0; // error
		}
		
	}
	else if( strcmp( parts[0], "Enemy") == 0 )
	{
		if( count != 4 )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG(" Enemy needs 4 parameters\n");
			return 0; // error
		}

		if( !is_valid_number( parts[1] ) )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		
		if( !is_valid_number( parts[2] ) )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		
		if( !is_valid_number( parts[3] ) )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}

		if( atoi( parts[3] ) == ENEMY_AF373 )
		{
			AddEnemy( atoi( parts[1] ), atoi( parts[2] ), atoi( parts[3] ) );
		}
		else
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid Enemy type\n", parts[3] );
			return 0; // error
		}
	}
	else if( strcmp( parts[0], "Player") == 0 )
	{
		if( count != 3 )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG(" Player needs 3 parameters\n");
			return 0; // error
		}
		if( !is_valid_number( parts[1] ) )
		{
			DEBUGLOG( "%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		if( !is_valid_number( parts[2] ) )
		{
			DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}

		pPlayer->SetPos( (double) atoi( parts[1] ), (double)window_height - (double) atoi( parts[2] ) );
		
		pPlayer->Startposx = (double) atoi( parts[1] );
		pPlayer->Startposy = (double)window_height - (double) atoi( parts[2] );
	}
	else if( strcmp( parts[0], "Music") == 0 )
	{
		string filename = MUSIC_DIR;
		filename += parts[1];

		if( !FileValid( filename ) ) 
		{
			DEBUGLOG( "%s : line %d Error : ", levelfile.c_str() , line );
			DEBUGLOG( "%s music file does not exist\n", parts[1] );
			return 0; // error			
		}

		pLevelData_Layer1->Musicfile = parts[1];
	}
	else
	{
		DEBUGLOG("%s : line %d Error : ", levelfile.c_str() , line );
		DEBUGLOG(" Unknown definition : %s\n", parts[0] );
		return 0; // error
	}

	return 1; // Succesfull	
}