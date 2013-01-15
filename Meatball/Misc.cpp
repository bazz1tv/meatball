#include "Misc.h"

void UniversalEventHandler(SDL_Event *event)
{
	switch(event->type)
	{
		case SDL_VIDEORESIZE:
		{
			
			//Resize the screen
			Screen = SDL_SetVideoMode( event->resize.w, event->resize.h, pGameSettings->Screen_Bpp, MEATBALL_CUSTOM_FLAGS );
			//If there's an error
			if( Screen == NULL )
			{
				printf ("OOPS");
				//windowOK = false;
				return;
			}
			break;
		}
		case SDL_VIDEOEXPOSE:
		{
			//printf ("OOPS");
			//Update the screen
			if( SDL_Flip( Screen ) == -1 )
			{
				//If there's an error
				//windowOK = false;
				printf ("OOPS");
				return;
			}
			break;
		}
		case SDL_QUIT:
		{
			mode = MODE_QUIT;
			break;
		}
		case SDL_KEYDOWN:
		{
			if( event->key.keysym.sym == SDLK_RETURN && ( event->key.keysym.mod & KMOD_ALT ) ) 
			{
					SDL_ToggleFS(Screen);
			}

		}

	}
}


/// Queries the Screen if it's set to Full of Not
/// @return 0 if windowed, 1 if fullscreen
int IsFullScreen(SDL_Surface *surface) 
{ 
    if (surface->flags & SDL_FULLSCREEN) return 1; // return true if surface is fullscreen 
    return 0; // Return false if surface is windowed 
} 

/// Toggles On/Off FullScreen
int SDL_ToggleFS(SDL_Surface *surface) 
{ 
    Uint32 flags = surface->flags; // Get the video surface flags 
    
    if (IsFullScreen(surface)) 
    { 
        // Swith to WINDOWED mode 
        if ((surface = SDL_SetVideoMode(MEATBALL_WIN_SETTINGS, (flags | SDL_RESIZABLE) & ~SDL_FULLSCREEN)) == NULL) return 0; 
        return 1; 
    } 
    
    // Swith to FULLSCREEN mode 
    if ((surface = SDL_SetVideoMode(MEATBALL_FULLSCREEN_SETTINGS, (flags|SDL_FULLSCREEN))) == NULL) return 0; 
    return 1; 
} 

/// Check for Collision with the Mouse
/// @returns true for collision
bool MouseCollidesWith(SDL_Rect *Crect)
{
	return pMouse->CollisonCheck( Crect );
}