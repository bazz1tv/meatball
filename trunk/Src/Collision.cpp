
#include "Collision.h"
#include "sprite.h"
#include "level.h"
#include "enemy.h"
#include "player.h"
#include "Camera.h"
#include "Bullet.h"

class cPlayer;
extern cLevel *pLevel;
extern cPlayer *pPlayer;

void Collisiondata::Reset()
{
	iCollisionType = 0; // no Collision
	iCollisionNumber = 0;
	collide = -1;
	direction = -1;
	collision = SDL_FALSE;
}
void incremental_X_check(cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type)
{
	
}

void incremental_Y_check(cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type)
{
	
}

void CollideMove( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type )
{
    if (!Sprite->visible ||
        /* coords are not in camera view */
        ((Sprite->posx + Sprite->width) - pCamera->x) < -100 || (Sprite->posx - pCamera->x) > window.w + 100 ||
        ((Sprite->posy + Sprite->height) - pCamera->y) < -100 || (Sprite->posy - pCamera->y) > window.h + 100)
    {
        return;
    }

    Collisiondata lastCollision;
    lastCollision = *Collision;
    
	Collision->Reset();

	double posx_old = Sprite->posx;
	double posy_old = Sprite->posy;
	double fvelx = velx;
	double fvely = vely;
	// these t_values are used in the movex,movey loops, incase we start incrementing/decrementing (approaching) the collision object (so we can get to its edge,
	// instead of being inside it)
	// we don't know whether the collision will be found or not, so we track the increment/decrements with tvelx, and if we've done enough incrementing,
	// to reach what the final sprite position would be, then we stop there and "call it a day"
	double tvelx=0,tvely=0;
    double incx=0, incy=0;
    if (fvelx > 0.0)
        incx = fvelx < 1 ? fvelx : 1;
    else if (fvelx < 0.0)
        incx = fvelx > -1.0 ? fvelx : -1;
    if (fvely > 0.0)
        incy = fvely < 1 ? fvely : 1;
    else if (fvely < 0.0)
        incy = fvely > -1.0 ? fvely : -1;
	
	SDL_bool movex = SDL_FALSE;
	SDL_bool movey = SDL_FALSE;
	
	if( vely != 0.0 )
	{
		movey = SDL_TRUE;
	}
	
	if( velx != 0.0 )
	{
		movex = SDL_TRUE;
	}
	
	while( movey == SDL_TRUE || movex == SDL_TRUE ) // While there is movement
	{
		if( movex == SDL_TRUE )
		{
			PositionCheck( (int)( Sprite->posx + incx ), (int)Sprite->posy, (int)Sprite->width, (int)Sprite->height, Collision, type  );
            
            /*if (type == SPRITE_TYPE_BULLET && Collision->iCollisionType == SPRITE_TYPE_ENEMY && static_cast<cBullet *>(Sprite)->Origin == SPRITE_TYPE_ENEMY)
                Collision->collision = SDL_FALSE;*/
            tvelx += incx;
			
			if( !Collision->collision)	// if no collisions
			{
				Sprite->posx += incx; // Add the one
                
                if ((fvelx > 0.0 && tvelx >= fvelx) || (fvelx < 0 && tvelx <= fvelx))
                    movex = SDL_FALSE;
				
				// But if the actual velocity is < 1, add that lesser value instead of 1
				if((Sprite->posx > posx_old + fvelx && fvelx > 0.0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
				{
					Sprite->posx = posx_old + fvelx;
				}
			}
			else	// there was a collision
			{
				if (velx > 0.0)
				{
					Collision->collide = LEFT;
				}
				else
					Collision->collide = RIGHT;
				
				
				
				if( Collision->direction == ALL_COLLISIONS_NONE )
				{
					Collision->direction = ALL_COLLISIONS_LR;	// Collision Left/Right
				}
				else if( Collision->direction == ALL_COLLISIONS_UD )
				{
					Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
				}
				
				movex = SDL_FALSE;
			}
		}
		
		if( movey == SDL_TRUE )
		{
            PositionCheck( (int)Sprite->posx, (int)( Sprite->posy + incy ), (int)Sprite->width, (int)Sprite->height, Collision, type );
            tvely += incy;
            
            if( !Collision->collision )
			{
				Sprite->posy += incy;
				
				if ((fvely > 0.0 && tvely >= fvely) || (fvely < 0.0 && tvely <= fvely))
					movey = SDL_FALSE;
				
				if((Sprite->posy > posy_old + fvely && fvely > 0.0) || (Sprite->posy < posy_old + fvely && fvely < 0.0))
				{
					Sprite->posy = posy_old + fvely;
                    movey = SDL_FALSE;
				}
			}
			else
			{
				if (vely > 0.0)
				{
					Collision->collide = DOWN;
				}
				else
					Collision->collide = UP;
				
				if( Collision->direction == ALL_COLLISIONS_NONE )
				{
					Collision->direction = ALL_COLLISIONS_UD;	// Collision Up/Down
				}
				else if( Collision->direction == ALL_COLLISIONS_LR )
				{
					Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
				}
				
				movey = SDL_FALSE;
			}
		}
	}
}


void CollideMove_Weak( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type )
{
	Collision->Reset();
	PositionCheck( (int)( Sprite->posx + velx ) , (int)( Sprite->posy + vely ), (int)(Sprite->width), (int)Sprite->height, Collision, type );
	
	if( !Collision->collision )
	{
	 
		Sprite->posx += velx;
		Sprite->posy += vely;

	}
	else
	{
		double posx_old = Sprite->posx;
		double posy_old = Sprite->posy;
		double fvelx = velx;
		double fvely = vely;
		// these t_values are used in the movex,movey loops, incase we start incrementing/decrementing (approaching) the collision object (so we can get to its edge,
		// instead of being inside it)
		// we don't know whether the collision will be found or not, so we track the increment/decrements with tvelx, and if we've done enough incrementing,
		// to reach what the final sprite position would be, then we stop there and "call it a day"
		double tvelx=0,tvely=0;
		
		SDL_bool movex = SDL_FALSE;
		SDL_bool movey = SDL_FALSE;
		
		if( vely != 0 )
		{
			movey = SDL_TRUE;
		}
		
		if( velx != 0 )
		{
			movex = SDL_TRUE;
		}
		
		while( movey == SDL_TRUE || movex == SDL_TRUE ) // While there is movement
		{
			if( movex == SDL_TRUE )
			{
				PositionCheck( (int)( Sprite->posx + ( (fvelx > 0) ? (tvelx+=1) : (tvelx+=-1) ) ), (int)Sprite->posy, (int)Sprite->width, (int)Sprite->height, Collision, type  );
				
				if ((fvelx > 0 && tvelx >= fvelx) || (fvelx < 0 && tvelx <= fvelx))
					movex = SDL_FALSE;
				
				if( !Collision->collision)	// if no collisions
				{
					Sprite->posx += ((fvelx > 0) ? (1) : (-1)); // Add the one
					
					// But if the actual velocity is < 1, add that lesser value instead of 1
					if((Sprite->posx > posx_old + fvelx && fvelx > 0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
					{
						Sprite->posx = posx_old + fvelx;
					}
				}
				else	// there was a collision
				{
					if (velx > 0)
					{
						Collision->collide = LEFT;
					}
					else
						Collision->collide = RIGHT;
					
					
					
					if( Collision->direction == ALL_COLLISIONS_NONE )
					{
						Collision->direction = ALL_COLLISIONS_LR;	// Collision Left/Right
					}
					else if( Collision->direction == ALL_COLLISIONS_UD )
					{
						Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
					}
					
					movex = SDL_FALSE;
				}
			}
			
			if( movey == SDL_TRUE )
			{
				if( !Collision->collision )
				{
					Sprite->posy += ((fvely > 0) ? (tvely+=1) : (tvely+=-1));
					
					if ((fvely > 0 && tvely >= fvely) || (fvely < 0 && tvely <= fvely))
						movey = SDL_FALSE;
					
					if((Sprite->posy > posy_old + fvely && fvely > 0) || (Sprite->posy < posy_old + fvely && fvely < 0))
					{
						Sprite->posy = posy_old + fvely;
					}
				}
				else
				{
					if (vely > 0)
					{
						Collision->collide = UP;
					}
					else
                    {
						Collision->collide = DOWN;
                    }
					
					if( Collision->direction == ALL_COLLISIONS_NONE )
					{
						Collision->direction = ALL_COLLISIONS_UD;	// Collision Up/Down
					}
					else if( Collision->direction == ALL_COLLISIONS_LR )
					{
						Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
					}
					
					movey = SDL_FALSE;
				}
			}
		}
	}
}


// Needs to get Documented
void PositionCheck( int x, int y, int width, int height, Collisiondata *Collision, unsigned int type )
{
	SDL_Rect rect1 = SetRect( x, y, width, height );
	SDL_Rect camrect = SetRect(pCamera->x, pCamera->y, window.w, window.h);
	//if (!RectIntersect(&rect1, &camrect))
	//	return;

	register unsigned int i;

	Collision->collision=SDL_FALSE;
	Collision->iCollisionNumber = 0;
	Collision->iCollisionType = 0;
	Collision->collide = -1;

	//if (width == 0 || height == 0)
		//return;
	
	// Check for an Collision with the Player

	if( type != SPRITE_TYPE_PLAYER && type != SPRITE_TYPE_PARTICLE && pPlayer->visible )
	{
		if( RectIntersect( &rect1, &pPlayer->rect ) )  // if Position is not valid
		{
			Collision->iCollisionNumber = 0;
			Collision->iCollisionType = SPRITE_TYPE_PLAYER;
			Collision->collision = SDL_TRUE; // GetCollide( &rect1, &pPlayer->rect );
			Collision->cRect = &pPlayer->rect;
			return;
		}
	}

	// Check for an Collision with the Level

	if( type == SPRITE_TYPE_PLAYER || type == SPRITE_TYPE_PARTICLE || type == SPRITE_TYPE_ENEMY || type == SPRITE_TYPE_BULLET ) 
	{
		// This seems to check all level sprites.. This should be optimized to only check for collision with on screen sprites
		// Check for an Collision with an LevelObject
		for( i = 0;i < pLevel->pLevelData_Layer1->BasicSprites.objcount;i++ )
		{
            cMVelSprite *cmvs = pLevel->pLevelData_Layer1->BasicSprites.objects[i];
            if( !cmvs || /* coords are not in camera view */
                             ((cmvs->posx + cmvs->width) - pCamera->x) < -150 || (cmvs->posx - pCamera->x) > window.w + 150 ||
                             ((cmvs->posy + cmvs->height) - pCamera->y) < -150 || (cmvs->posy - pCamera->y) > window.h + 150)
			{
				continue;
			}
			// or if this object isn't in the current screen space
			//if (!RectIntersect(&pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect, &camrect))
				//return;

			if( pLevel->pLevelData_Layer1->BasicSprites.objects[i]->type == SPRITE_TYPE_SOLID )
			{
				if( RectIntersect( &rect1, &pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect ) ) // if Position is not valid
				{
					Collision->iCollisionNumber = i;
					Collision->iCollisionType = SPRITE_TYPE_SOLID;	// Massive Object
					Collision->collision = SDL_TRUE; //GetCollide( &rect1, &pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect );
					Collision->cRect = &pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect;
					return;
				}
			}

			if( pLevel->pLevelData_Layer1->BasicSprites.objects[i]->type == SPRITE_TYPE_TOPSOLID ) // if Position is not valid
			{
				if( RectIntersect( &rect1, &pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect ) )
				{
					Collision->iCollisionNumber = i;
					Collision->iCollisionType = SPRITE_TYPE_TOPSOLID;	// HalfMassive Object
					Collision->collision = SDL_TRUE; //GetCollide( &rect1, &pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect );
					Collision->cRect = &pLevel->pLevelData_Layer1->BasicSprites.objects[i]->rect;
					return;
				}
			}
		}
	}

	// Check for an Collision with the enemies

	if( type == SPRITE_TYPE_PLAYER || type == SPRITE_TYPE_PARTICLE || type == SPRITE_TYPE_BULLET ) 
	{
		for( i = 0;i < Enemies.objcount;i++ )
		{
            /* NOTE / TODO : The 150 below relates to 100 which comes from 1/8 of 800 from 800x600 screen resolution. Come to think of it, the Y values should be matched from 600, not 800..
             
             Anyways, the 100 was specified in sprite update routines such that any sprite that is offscreen by more than 100 pixels will not be updated. However, if a sprite within this limit does a collision check, there was a bug such that we must expand on that limitation to make sure we are checking collisions against tiles adjacent to that sprite
             
            For example, if a sprite was at the very limit of the px checked in sprite update routine (eg. CmVelSprite::Update, he needs to be able to check against collision tiles that are at the extremity plus his velocity. So, 50px is a pretty kind value.
             
             TODO: Use actual fractions instead of hardcoded pixel values. That way the relationships stay the same if we ever journey outside of 800x600. */
            cEnemy *e = Enemies.objects[i];
            if( !e || !e->visible || /* coords are not in camera view */
               ((e->posx + e->width) - pCamera->x) < -150 || (e->posx - pCamera->x) > window.w + 150 ||
               ((e->posy + e->height) - pCamera->y) < -150 || (e->posy - pCamera->y) > window.h + 150)
			{
				continue;
			}
			// or if this object isn't in the current screen space
			//if (!RectIntersect(&Enemies.objects[i]->rect, &camrect))
				//return;

			if( RectIntersect( &rect1, &Enemies.objects[i]->rect ) ) // if Position is not valid
			{
				Collision->iCollisionNumber = i;
				Collision->iCollisionType = SPRITE_TYPE_ENEMY;	// Enemy Object
				Collision->collision = SDL_TRUE; // GetCollide( &rect1, &Enemies.objects[i]->rect );
				Collision->cRect = &Enemies.objects[i]->rect;
				return;
			}
		}
	}

}



//Stub
