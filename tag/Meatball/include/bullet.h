#ifndef __BULLET_H__
#define __BULLET_H__

/// Bullet types
#define BULLET_UNKNOWN			0
#define BULLET_PISTOL			1
#define BULLET_MACHINEGUN		2
#define BULLET_LASER_X1			3

class cBullet : public cMVelSprite
{
public:
	cBullet( double nposx , double nposy );
	~cBullet( void );

	/** Initializes the Bullet with the given parameters
	 * Reinitializes all values
	 */
	void init( int ndirection, unsigned int nBullet_type, unsigned int nOrigin, double nextravel = 0.0 );

	/// Updates the Bullet
	void Update( void );
	
	/// Draws the Bullet
	void Draw( SDL_Surface *target );

	/// The Bullet Type
	unsigned int Bullet_type;

	/// The Origin can be Player,Enemy  or Static ( Todo )
	unsigned int Origin;

	/// The Direction
	int direction;
	
	/// The Bullet Explosion Sound
	Mix_Chunk *sound_Explosion;

};

extern unsigned int BulletCount;
extern cBullet **Bullets;

/** Preloads all Bullet images
 * get the pointer with the image manager and the image path
 */
void PreloadBulletimages( void );

/** Adds an Bullet which gets controlled by the game
 * just use Bullet type
 */
void AddBullet( double nposx, double nposy, int ndirection, unsigned int nBullet_type, unsigned int nOrigin, double nAddVel = 0.0 );

/// Updates all Bullets
void UpdateBullets( void );

/// Draws all Bullets
void DrawBullets( void );

/// Deletes all Bullets ( Cleans the Array )
void DeleteAllBullets( void );

#endif