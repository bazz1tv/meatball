
#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Globals.h"

typedef struct
{
	/* The first direct Collision direction
	 * -1 : No Collision
	 * 0  : Left
	 * 1  : Right
	 * 2  : Up
	 * 3  : Down
	 */
	int collide;

	/* Collisiontypes
	 * see SPRITE_TYPE defines
	 */
	int iCollisionType;

	// The Number of the Collidiong Sprite
	int iCollisionNumber;

	/* The direction of all Collisions
	 * -1 = No Collision detected
	 * 1 = Collision Up/Down/Left/Right
	 * 2 = Collision in Left/Right
	 * 3 = Collision Up/Down
	 */	
	int direction;

} Collisiondata;

// Returns the Collision data if an Collision happened
void CollideMove( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type );

void CollideMove_Meatball( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type );
/*	Checks if the given position is valid
 *	if the Position is not valid it returns the direction
 */
void PositionCheck( int x, int y, int width, int height, Collisiondata *Collision, unsigned int type );

// Returns the collision direction
int GetCollide( SDL_Rect *r1, SDL_Rect *r2 );

#endif