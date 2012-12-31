
#include "include\Globals.h"

cCamera :: cCamera( void )
{
	x = 0.0;
	y = 0.0;

	// default limits
	Lleft = 0;
	Lright = 10000;
}

cCamera :: ~cCamera( void )
{

}

void cCamera :: Update( void )
{
	if( mode == MODE_GAME ) 
	{
		
		if( (pPlayer->posx - Screen->w) > (x - 300) ) // Right
		{
			x += ((pPlayer->posx - x + 300 - Screen->w)/10)*pFramerate->speedfactor; // nice and smooth ;)
		}
		else if( pPlayer->posx < (x + 300) ) // Left
		{
			x += ((pPlayer->posx - x - 300)/10)*pFramerate->speedfactor;	// nice and smooth ;)
		}

		Limits_Check();
	}
}

void cCamera :: SetPos( double nx, double ny )
{
	x = nx;
	y = ny;

	if( mode == MODE_GAME ) 
	{
		Limits_Check();
	}
}

void cCamera :: Move( double ax, double ay )
{
	x += ax;
	y += ay;

	if( mode == MODE_GAME ) 
	{
		Limits_Check();
	}
}


void cCamera ::Limits_Check( void )
{
	if( x > (double)Lright ) 
	{
		x = (double)Lright;
	}
	else if( x < (double)Lleft ) 
	{
		x = (double)Lleft;
	}

	if( y > 0 ) 
	{
		y = 0;
	}
}