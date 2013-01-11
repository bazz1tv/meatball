
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "include\Globals.h"

#define STATE_STAYING 0
#define STATE_WALKING 1
#define STATE_JUMPING 2
#define STATE_FALLING 3
#define STATE_FLYING  4

/// The MeatBall Player class
class cPlayer : public cMVelSprite
{
public:
	cPlayer( double nposx = 0 , double nposy = 0 );
	~cPlayer( void );
	
	/// Loads the Player images
	void init( void );
	
	/// Updates the Player
	void Update( void );

	/// Draws the Player
	void Draw( SDL_Surface *target );

	/// Resets
	void Reset( void );

	/// Jump with the given Power
	void Jump( int power = 5 );

	/// Chnage the Active Weapon
	void ChangeActiveWeapon( unsigned int Weapon_type );

	/// Fires the specified Bullet
	void Fire( void );

	/// The Player gets hit by an Bullet or something else
	void Get_Hit( unsigned int damage );

	/// The Player dies
	void Die( void );

	/**	The images
	 *	0 = Standing Left
	 *	1 = Standing Right
	 *  2 = Standing Looking up
	 *	3 = Ducking
	 */
	SDL_Surface *images[5];

	/// The Player Weapon
	cWeapon *pWeapon;

	/**
	 * 0 = facing left
	 * 1 = facing right
	 * 2 = facing up
	 * 3 = facing down
	 *  we need to add angle shots too.. 
	 *  we'll add when player possesses these abilities
	 */	
	int direction;

	/** 0 : falling
	 * 1 : onground massive
	 * 2 : onground halfmassive
	 */
	int onGround;

	/** 0 = Staying ( not doing anything :)
	 * 1 = Moving,Walking
	 * 2 = Jumping
	 * 3 = Falling
	 * 4 = Flying
	 */	
	int state;

	/** If you press the Jump Button before you
	 * touch the ground you have this time
	 * to touch the ground and then jump
	*/
	double jump_timer;

	/** Power of the Jump Timer given
	 * when pressed the Jump Button
	 */ 
	int jump_timer_power;

	double jumpvel;

	/** The Weapon which the Player has in his hands ( Active Weapon )
	 * see Weapon definitions
	 */ 
	unsigned int actWeapon;

	/** The Health
	 * if == 0 the player dies and the level restarts ;O
	 */
	unsigned int Health;

	unsigned int updown_pressed; ///< Up or Down -> Left or Right 
};

void UpdatePlayer( void );

#endif