/// @addtogroup Camera
// @{
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "include\Globals.h"

/// The Camera class
class cCamera
{
public:
	cCamera( void );
	~cCamera( void );
	
	/// Sets the Camera Position
	void SetPos( double nx, double ny );
	/// Moves the Camera
	void Move( double ax, double ay );
	
	void SetLimits( int nleft,int nright ); ///< Not Implemented (yet)

	/// Update the Camera
	void Update( void );
	
	/// The Camera Position
	double x, y;
	/// Camera Limits
	int Lleft, Lright;

	///
	///bool camera_moved;

private:
	void Limits_Check( void );
};

#endif
// @}