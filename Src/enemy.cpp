
#include "enemy.h"
#include "AF-373.h"
#include "Collision.h"
#include "effects.h"
#include "Camera.h"

//unsigned int EnemyCount = 0;
//cEnemy **Enemies = NULL;
ObjectManager<cEnemy> Enemies(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS);


cEnemy :: cEnemy( double nposx, double nposy )
: cMVelSprite( NULL, nposx, nposy )
{
	type = SPRITE_TYPE_ENEMY;

	Enemy_type = ENEMY_UNDEFINED;
	Enemy_state = ENEMY_AI_STATE_UNKNOWN;

	Health = 10;
	
	SetPos( nposx, nposy );
}

cEnemy :: ~cEnemy( void )
{
	// ...
}

void cEnemy :: init( void )
{
	// virtual
	visible = SDL_TRUE;
}

void cEnemy :: Update( void )
{
	cMVelSprite :: Update();
}

void cEnemy :: Draw( SDL_Renderer *renderer )
{
	// virtual 
}

void cEnemy :: Jump( int power /** = 5  */ )
{
	// virtual 
}

void cEnemy::Get_Hit(int damage)
{
	DEBUGLOG("cEnemy::Get_Hit\n");
	if ( (Health - damage) <= 0)
	{
		Health = 0;
		Die();
	}
	else
	{
		Health -= damage;
	}
}

void cEnemy :: Die( void )
{
	// virtual
	DEBUGLOG("cEnemy::Die\n");
	int explode_posx, explode_posy;
	/*DEBUGLOG ("\tCollision direction : %d\n", Collision->collide);
	DEBUGLOG ("\tCollisions directons: %d\n", Collision->direction);*/

	explode_posx = posx + (width / 2);
	explode_posy = posy + (height / 2);

	visible = SDL_FALSE;

	AddParticleEmitter(explode_posx, explode_posy, 4, 255, 225, 255, 4, 30, 10);
	pAudio->PlaySound(SMan->GetPointer("dry-explosion-fx"));
}

cEnemy * AddEnemy( double nposx, double nposy, unsigned int etype )
{
	DEBUGLOG("AddEnemy\n");
	for( register unsigned int i = 0; i < Enemies.objcount; i++ )
	{
		if (Enemies.objects[i])
		{
			DEBUGLOG("\t1 ");
			if ( !Enemies.objects[i]->visible && Enemies.objects[i]->Enemy_type == etype)
			{
				DEBUGLOG("2 ");
				Enemies.objects[i]->SetPos( nposx, nposy );
				
				Enemies.objects[i]->init();
				
				return Enemies.objects[i];
			}
		}
	}

	DEBUGLOG("\t3 ");
	cEnemy *new_Enemy = NULL;

	if( etype == ENEMY_AF373 )// Enemy type 
	{
		new_Enemy = new cAF373( nposx, nposy );		
	}
	else
	{
		DEBUGLOG( "Warning : Unknown Enemy Type : %d\n", etype );
		return NULL;
	}

	new_Enemy->init();	

	Enemies.add(new_Enemy);
	//Enemies = (cEnemy**) SDL_realloc( Enemies, ++EnemyCount * sizeof(cEnemy*) );
	//Enemies[EnemyCount - 1] = new_Enemy;
    return new_Enemy;
}

void UpdateEnemies( void )
{
	for( register unsigned int i = 0; i < Enemies.objcount; i++ )
	{
		cEnemy *e = Enemies.objects[i];
		if (!e || !e->visible ||
			/* coords are not in camera view */
			((e->posx + e->width) - pCamera->x) < -100 || (e->posx - pCamera->x) > window.w + 100 ||
			((e->posy + e->height) - pCamera->y) < -100 || (e->posy - pCamera->y) > window.h + 100)
		{
			continue;
		}
		
		CollideMove( (cBasicSprite*)Enemies.objects[i], Enemies.objects[i]->velx * pFramerate->speedfactor, Enemies.objects[i]->vely * pFramerate->speedfactor, Enemies.objects[i]->Collision, Enemies.objects[i]->type );
		Enemies.objects[i]->Update();
	}
}

int GetCollidingEnemyNum( SDL_Rect *Crect )
{
	if( Enemies.objects && Crect )
	{
		for( register unsigned int i = 0; i < Enemies.objcount; i++ )
		{
			if( !Enemies.objects[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)Enemies.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				return i;
			}
		}
	}

	return -1;
}

// This function takes external pointers
SDL_bool GetAllCollidingEnemyNum( SDL_Rect *Crect, ObjectManager<cMVelSprite> &obj_man )
{
	SDL_bool did_we_get_objects = SDL_FALSE;
	
	if( Enemies.objects && Crect )
	{
		//DEBUGLOG( "Using Crect coords x: %d, y: %d, width: %d, height: %d\n", Crect->x,Crect->y, Crect->w, Crect->h);
		
		for( register unsigned int i = 0; i < Enemies.objcount; i++ )
		{
			if( !Enemies.objects[i] )
			{
				continue;
			}
            
			if( RectIntersect( &(const SDL_Rect&)Enemies.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				// Add this sprite num to the list
				if (obj_man.hasa(Enemies.objects[i]) < 0)
					obj_man.add(Enemies.objects[i]);
				
				did_we_get_objects = SDL_TRUE;
			}
		}
	}
	
	return did_we_get_objects;
}

cEnemy *GetCollidingEnemy( SDL_Rect *Crect )
{
	if( Enemies.objects && Crect )
	{
		for( register unsigned int i = 0; i < Enemies.objcount; i++ )
		{
			if( !Enemies.objects[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)Enemies.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				return Enemies.objects[i];
			}
		}
	}

	return NULL;
}

void DrawEnemies( SDL_Renderer *renderer )
{
	for( register unsigned int i = 0; i < Enemies.objcount; i++ )
	{
		if( !Enemies.objects[i] || !Enemies.objects[i]->visible ) 
		{
			continue;
		}
		
		Enemies.objects[i]->Draw( renderer );
	}
}

void DeletEnemy( unsigned int number )
{
	if( Enemies.objects && number < Enemies.objcount && Enemies.objects[number] )
	{
		delete Enemies.objects[number];
		Enemies.objects[number] = NULL;
	}
}

void DeleteAllEnemies( void )
{	
	Enemies.~ObjectManager();
}