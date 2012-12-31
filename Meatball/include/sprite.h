
#ifndef __SPRITE_H__
#define __SPRITE_H__

#define SPRITE_TYPE_NO_TYPE		0
#define SPRITE_TYPE_PLAYER		1
#define SPRITE_TYPE_MASSIVE		2
#define SPRITE_TYPE_PASSIVE		3
#define SPRITE_TYPE_HALFMASSIVE 4
#define SPRITE_TYPE_BULLET		5
#define SPRITE_TYPE_PARTICLE	6
#define SPRITE_TYPE_ENEMY		7

class cMVelSprite : public cVelocitySprite
{
public:
	cMVelSprite( SDL_Surface *new_image, double x, double y, double nvelx = 0, double nvely = 0 );
	~cMVelSprite( void );

	void Update( void );

	void Draw( SDL_Surface *target );

	SDL_Rect GetRect( bool with_camerapos = 0 );

	/* The Type
	 * 0 = No Type
	 * 1 = Player
	 * 2 = Sprite Massive
	 * 3 = Sprite Passive
	 * 4 = Sprite Halfmassive
	 * 5 = Sprite Bullet
	 * 6 = ?
	 * 7 = ?
	 *
	 */
	int type;
	
	// Start Position
	double Startposy, Startposx;

	Collisiondata *Collision;
};

class cMAngleSprite : public cAngleSprite
{
public:
	cMAngleSprite( SDL_Surface *new_image, double x, double y, double nangle = 0, double nspeed = 0 );
	~cMAngleSprite( void );

	void Update( void );

	void Draw( SDL_Surface *target );

	SDL_Rect GetRect( bool with_camerapos = 0 );

	/* The Type
	 * 0 = No Type
	 * 1 = Player
	 * 2 = Sprite Massive
	 * 3 = Sprite Passive
	 * 4 = Sprite Halfmassive
	 * 5 = Sprite Bullet
	 * 6 = ?
	 * 7 = ?
	 *
	 */
	int type;
	
	// Start Position
	double Startposy, Startposx;

	Collisiondata *Collision;
};

#endif