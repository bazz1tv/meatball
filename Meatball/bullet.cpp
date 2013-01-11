#include "include\Globals.h"

unsigned int BulletCount = 0;
cBullet **Bullets = NULL;

cBullet :: cBullet( double nposx, double nposy )
: cMVelSprite( NULL, nposx, nposy )
{
	direction = RIGHT;

	Bullet_type = BULLET_UNKNOWN;	// no type
	type = SPRITE_TYPE_BULLET;
	
	Origin = 0; // Unknown

	visible = false;

	sound_Explosion = NULL;
}

cBullet :: ~cBullet( void )
{
	// Nothing
}

void cBullet :: init( int ndirection, unsigned int nBullet_type, unsigned int nOrigin, double nextravel /** = 0.0  */ )
{
	if( !nBullet_type ) 
	{
		visible = false;
		
		return;
	}

	double nVelocity = 0; // The Velocity

	Bullet_type = nBullet_type; // Set the Type
	direction = ndirection; // Set the Direction

	if( Bullet_type == BULLET_PISTOL ) // The Pistol Bullet
	{
		nVelocity = 11.5;
		
		SetImage( IMan->GetPointer( PIXMAPS_DIR "game/weapons/pistol/default.png" ) );
	}
	else if( Bullet_type == BULLET_MACHINEGUN )  // The Machinegun Bullet
	{
		nVelocity = 15;
		
		SetImage( IMan->GetPointer( PIXMAPS_DIR "game/weapons/machinegun/default.png" ) );
	}
	else if( Bullet_type == BULLET_LASER_X1 )  // The Laser X-1
	{
		nVelocity = 17;
		
		if( direction == LEFT ) 
		{
			SetImage( IMan->GetPointer( PIXMAPS_DIR "game/weapons/Laser_X1/left.png" ) );
		}
		else if( direction == RIGHT ) 
		{
			SetImage( IMan->GetPointer( PIXMAPS_DIR "game/weapons/Laser_X1/right.png" ) );
		}
		else
		{
			printf( "Warning : Unsupported Laser X-1 direction used : %d\n", direction );

			visible = false;
			return;
		}
	}
	else
	{
		printf( "Warning : Unknown Bullet type used : %d\n", Bullet_type );

		visible = false;
		return;
	}

	if( direction == RIGHT ) 
	{
		SetVelocity( nVelocity + nextravel, 0 );
	}
	else if( direction == LEFT ) 
	{
		SetVelocity( -( nVelocity + nextravel ), 0 );

		posx -= width;
	}
	else if( direction == UP ) 
	{
		SetVelocity( 0, -( nVelocity + nextravel ) );
	}
	else if( direction == DOWN ) 
	{
		SetVelocity( 0, nVelocity + nextravel );	
	}
	else
	{
		printf( "Warning : Unknown Bullet direction used : %d\n", direction );

		visible = false;
		return;
	}
	
	Origin = nOrigin;

	posy -= height/2;

	visible = true;
}

void cBullet :: Update( void )
{
	if( !visible )
	{
		return;
	}

	cMVelSprite::Update();

	if( !velx && !vely ) // not Moving
	{
		visible = false;
		return;
	}


	// If there's an Collision 
	if( Collision->direction != -1 || Collision->collide != -1 )
	{
		if( ( Collision->iCollisionType == SPRITE_TYPE_PLAYER && this->Origin == SPRITE_TYPE_ENEMY ) ||
			( Collision->iCollisionType == SPRITE_TYPE_ENEMY && this->Origin == SPRITE_TYPE_PLAYER ) ||
			( Collision->iCollisionType == SPRITE_TYPE_MASSIVE || Collision->iCollisionType == SPRITE_TYPE_HALFMASSIVE ) ) 
		{
			if( Bullet_type == BULLET_PISTOL )
			{
				AddParticleEmitter( posx + ( width/2 ), posy + ( height/2 ), Random( ( velx + vely)/16, ( velx + vely)/8 ), 255, 250, 0, 10, 20, 20 ); // Yellow
				visible = false;
			}
			else if( Bullet_type == BULLET_MACHINEGUN ) 
			{
				AddParticleEmitter( posx + ( width/2 ), posy + ( height/2 ), Random( ( velx + vely)/16, ( velx + vely)/8 ), 255, 150, 0, 11, 10, 30 ); // Red-Yellow
				visible = false;
			}
			else if( Bullet_type == BULLET_LASER_X1 ) 
			{
				AddParticleEmitter( posx + ( width/2 ), posy + ( height/2 ), Random( ( velx + vely)/8, ( velx + vely)/4 ), 0, 250, 0, 5, 10, 5 ); // Green
				visible = false;
			}
		}
	}	

	// Position Correction
	if( posx - width > (double)Screen->w + pCamera->x ) 
	{
		visible = false;
	}
	else if( posx < -width + pCamera->x )
	{
		visible = false;
	}

	if( posy < -height + pCamera->y )
	{
		visible = false;
	}
	else if( posy - height > (double)Screen->h + pCamera->y ) 
	{
		visible = false;
	}
}

void cBullet :: Draw( SDL_Surface *target )
{
	if ( !visible || !target || !image )
	{
		return;
	}

	rect.x = (int)(posx - pCamera->x);
	rect.y = (int)(posy - pCamera->y);

	SDL_BlitSurface( image, NULL, target, &rect );

	rect.x = (int)posx;
	rect.y = (int)posy;
}

void PreloadBulletimages( void )
{
	// Pistol
	//SDL_Surface *tmp = sge_transform_surface(LoadImage( PIXMAPS_DIR "game/weapons/pistol/default.png", colorkey ), colorkey, 0, 0.70f, 0.70f, 0);
	//if (tmp)
		//SDL_SetColorKey( tmp, SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_SRCALPHA, colorkey );
	//SDL_Surface *sge_transform_surface(SDL_Surface *src, Uint32 bcol, float angle, float xscale, float yscale, Uint8 flags)
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/pistol/default2.png", colorkey ),PIXMAPS_DIR "game/weapons/pistol/default.png" );
	// Machinegun
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/machinegun/default.png", colorkey ), PIXMAPS_DIR "game/weapons/machinegun/default.png" );
	// Laser X-1
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/Laser_X1/left.png", colorkey ), PIXMAPS_DIR "game/weapons/Laser_X1/left.png" );
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/Laser_X1/right.png", colorkey ), PIXMAPS_DIR "game/weapons/Laser_X1/right.png" );
}

void AddBullet( double nposx, double nposy, int ndirection, unsigned int nBullet_type, unsigned int nOrigin, double nextravel /** = 0.0  */ )
{
	for( unsigned int i = 0; i < BulletCount; i++ )
	{
		if ( !Bullets[i]->visible )
		{
			Bullets[i]->SetPos( nposx, nposy );
			
			Bullets[i]->init( ndirection, nBullet_type, nOrigin, nextravel );

			return;
		}
	}

	cBullet *new_Bullet = new cBullet( nposx, nposy );

	new_Bullet->init( ndirection, nBullet_type, nOrigin, nextravel );

	Bullets = (cBullet**) realloc( Bullets, ++BulletCount * sizeof(cBullet*) );
	Bullets[BulletCount - 1] = new_Bullet;
}

void UpdateBullets( void )
{
	for( unsigned int i = 0; i < BulletCount; i++ )
	{
		if( Bullets[i]->visible ) 
		{
			CollideMove( (cBasicSprite*)Bullets[i], Bullets[i]->velx * pFramerate->speedfactor, Bullets[i]->vely * pFramerate->speedfactor, Bullets[i]->Collision, Bullets[i]->type );
			Bullets[i]->Update();
		}
	}
}

void DrawBullets( void )
{
	for( unsigned int i = 0; i < BulletCount; i++ )
	{
		Bullets[i]->Draw( Screen );
	}
}

void DeleteAllBullets( void )
{
	if ( Bullets )
	{
		for( unsigned int i = 0;i < BulletCount;i++ ) 
		{
			if( !Bullets[i] ) 
			{
				continue;
			} 
			
			delete Bullets[i];
			Bullets[i] = NULL;
		}

		delete []Bullets;
		Bullets = NULL;
	}

	BulletCount = 0;
}