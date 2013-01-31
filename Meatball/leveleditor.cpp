
//#include "leveleditor.h"
#include "Globals.h"

// This is the amount of time to SLEEP when doing HELD_KEY events (so we don't work at the speed of light)
#define LEVELEDIT_SLEEPWAIT 100000

// Definitions for easy portability
#define SAVE_KEY SDLK_s
#define COPY_KEY SDLK_c
#define PASTE_KEY SDLK_v
#define FASTCOPY_KEY SDLK_f

#define MULTI_COLOR 0xff00ff00



cLevelEditor :: cLevelEditor( void )
{
	Mouse_command = MOUSE_COMMAND_NOTHING;

	Mouse_w = 0;
	Mouse_h = 0;

	Object = NULL;
	CopyObject = NULL;
	lastCopiedObject = NULL;
}

cLevelEditor :: ~cLevelEditor( void )
{

}

void cLevelEditor :: Update( void )
{
	PreUpdate();
	HoveredObject = SetRect( 0, 0, 0, 0 );

	if( Mouse_command == MOUSE_COMMAND_NOTHING || Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
	{
		cMVelSprite *Collision_Object = GetCollidingObject( pMouse->posx, pMouse->posy );

		if( Collision_Object ) 
		{
			HoveredObject = Collision_Object->GetRect( SDL_TRUE );
		}
	}
	// Integrate Single_tile_MOVING and MULTI_TILE_MOVING
	else if( Mouse_command == MOUSE_COMMAND_SINGLE_TILE_MOVING )
	{
		Object->SetPos( floor(pMouse->posx + Mouse_w + pCamera->x), floor(pMouse->posy + Mouse_h + pCamera->y) );
		
		Object->Startposx = floor(pMouse->posx + Mouse_w + pCamera->x);
		Object->Startposy = floor(pMouse->posy + Mouse_h + pCamera->y);
	}
	else if (Mouse_command == MOUSE_COMMAND_MOVING_MULTI_TILES )
	{
		for (int i=0; i < MultiObjects.numobjs; i++)
		{
			MultiObjects.objects[i]->SetPos( floor((pMouse->posx-multi_mouseXOffset) + (pCamera->x-multi_camXOffset) + MultiObjects.objects[i]->posx), floor((pMouse->posy - multi_mouseYOffset) + (pCamera->y-multi_camYOffset) + MultiObjects.objects[i]->posy) );
			
			MultiObjects.objects[i]->Startposx = floor(MultiObjects.objects[i]->posx);
			MultiObjects.objects[i]->Startposy = floor(MultiObjects.objects[i]->posy);
		}
		multi_mouseXOffset = pMouse->posx;
		multi_mouseYOffset = pMouse->posy;
		multi_camXOffset = pCamera->x;
		multi_camYOffset = pCamera->y;
	}

	PostUpdate();

}


void cLevelEditor :: Draw()
{
	Draw(Renderer);
}
void cLevelEditor :: Draw (SDL_Renderer *renderer)
{
	Uint32 Color;

	PreDraw();

	pPlayer->Draw( renderer );
	
	DrawEnemies(renderer);
	pMouse->Draw( renderer );


	if( Mouse_command != MOUSE_COMMAND_FASTCOPY ) 
	{
		Color = 0xffffffff; // White
	}
	else if (Mouse_command == MOUSE_COMMAND_FASTCOPY)
	{
		Color = 0xff7878ff; // Blue
	}
	
	// Draw the outline of the hovered Object
	OutlineHoveredObject(renderer, Color);
	
	// Only the draw the multi-tile outlines if we are not moving them (for accuracy when moving)
	if (Mouse_command != MOUSE_COMMAND_MOVING_MULTI_TILES || (veryfirst_multi_mouseXOffset == pMouse->posx && veryfirst_multi_mouseYOffset == pMouse->posy))
	{
		// Draw the Multi-outline (it will check automatically if we have multi tiles to draw)
		DrawOutlineAroundMultiSelectedTiles(renderer, MULTI_COLOR);
	}
	PostDraw();
}

void cLevelEditor :: DrawOutlineAroundMultiSelectedTiles(SDL_Renderer *renderer, Uint32 Color)
{
	//HoveredObject = SetRect( 0, 0, 0, 0 );
	
	// This is code to get a RECT from our sprite Tile
	if (multiple_objects_selected)
	{
		for (int i=0; i < MultiObjects.numobjs; i++)
		{
			// Draw a rect around each individual object
			
			// get its rect
			cMVelSprite *ptr = MultiObjects.objects[i];
			SDL_Rect orect = ptr->GetRect(SDL_TRUE);
			
			OutlineObject(renderer, Color, &orect);
		}
	}
}

void cLevelEditor :: OutlineObject(SDL_Renderer *renderer, Uint32 Color, SDL_Rect *object_rect)
{
	// The object_rect Shadow
	// top line
	FillRect(renderer, object_rect->x+1, object_rect->y+1, object_rect->w, 1, 0,0,0);
	// Left Line
	FillRect(renderer, object_rect->x+1, object_rect->y+1, 1, object_rect->h, 0,0,0);
	// Bottom Line
	FillRect(renderer, object_rect->x+1, object_rect->y+1+object_rect->h, object_rect->w, 1, 0,0,0);
	// Right Line
	FillRect(renderer, object_rect->x+1+object_rect->w, object_rect->y+1, 1, object_rect->h, 0,0,0);
	
	// The object_rect Outline (white or blue depending on Fastcopy or just a selection)
	// top line
	FillRectAlpha(renderer, object_rect->x, object_rect->y, object_rect->w, 1, Color);
	// Left Line
	FillRectAlpha(renderer, object_rect->x, object_rect->y, 1, object_rect->h, Color);
	// Bottom Line
	FillRectAlpha(renderer, object_rect->x, object_rect->y+object_rect->h, object_rect->w, 1, Color);
	// Right Line
	FillRectAlpha(renderer, object_rect->x+object_rect->w, object_rect->y, 1, object_rect->h, Color);
}

void cLevelEditor:: OutlineHoveredObject( SDL_Renderer *renderer, Uint32 Color)
{
	OutlineObject(renderer, Color, &HoveredObject);
}


void cLevelEditor :: SetCopyObject( cMVelSprite *nObject )
{	
	if( !nObject ) 
	{
		return;
	}

	// Player can not be copied
	if( nObject->type == SPRITE_TYPE_PLAYER ) 
	{
		return;
	}

	CopyObject = nObject;
}

void cLevelEditor :: SetCopyObject( void )
{
	// Only Map and Enemy Objects can be Copied
	SetCopyObject( GetCollidingObject( pMouse->posx, pMouse->posy ) );
}

void cLevelEditor :: SetMultiObjects( void )
{
	// Only Map and Enemy Objects can be Copied (FOR NOW)
	SetMultiObjects( GetCollidingObject( pMouse->posx, pMouse->posy )  );
}

void cLevelEditor :: SetMultiObjects( cMVelSprite *nObject)
{
	if( !nObject )
	{
		return;
	}
	
	if (!MultiObjects.hasa(nObject))
	{
		MultiObjects.add(nObject);
	}
	
	
	multiple_objects_selected = SDL_TRUE;

	
}

void cLevelEditor::MultiMove(void)
{
	
}

void cLevelEditor :: SetMoveObject( cMVelSprite *nObject )
{
	if( !nObject ) 
	{
		return;
	}

	Mouse_w = (int)(nObject->posx -= pMouse->posx + pCamera->x);
	Mouse_h = (int)(nObject->posy -= pMouse->posy + pCamera->y);
	
	Object = nObject;

	Mouse_command = MOUSE_COMMAND_SINGLE_TILE_MOVING;
}

void cLevelEditor :: SetMoveObject( void )
{
	// Everything can be moved
	SetMoveObject( GetCollidingObject( pMouse->posx, pMouse->posy )  );
}



void cLevelEditor :: PasteObject( void )
{
	PasteObject( pMouse->posx, pMouse->posy );
}

void cLevelEditor :: PasteObject( double x, double y )
{
	if( !CopyObject ) 
	{
		return;
	}

	// Add the New Object
	if( CopyObject->type == SPRITE_TYPE_MASSIVE || CopyObject->type == SPRITE_TYPE_PASSIVE || CopyObject->type == SPRITE_TYPE_HALFMASSIVE) 
	{
		// Create the new Sprite
		cMVelSprite *new_Object = new cMVelSprite( CopyObject->srcimage, x + pCamera->x, y + pCamera->y );

		new_Object->type = CopyObject->type;

		pLevel->pLevelData->AddSprite( new_Object );

		if( Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
		{
			SetFastCopyObject( new_Object );
		}
		lastCopiedObject = new_Object;
	}
	else if( CopyObject->type == SPRITE_TYPE_ENEMY )
	{
		cEnemy *pEnemy = (cEnemy *)CopyObject;

		AddEnemy(x + pCamera->x, y + pCamera->y, pEnemy->Enemy_type );
	}
}

void cLevelEditor :: DeleteObject( void )
{
	printf("Delete Object Called\n");
	DeleteObject( pMouse->posx, pMouse->posy );
}

void cLevelEditor :: DeleteObject( double x, double y )
{
	// Only Map Objects and Enemies can be deleted
	
	SDL_Rect cRect = SetRect( (int)x, (int)y, 1, 1 );

	int CollisionNum = -1;

	// Map Objects	
	CollisionNum = pLevel->pLevelData->GetCollidingSpriteNum( &cRect );

	if( CollisionNum >= 0 )
	{
		printf("Collision Detected. Deleting Sprite\n");
		pLevel->pLevelData->DeleteSprite( CollisionNum );
		return;
	}
	
	CollisionNum = GetCollidingEnemyNum( &cRect );

	if( CollisionNum >= 0 )
	{
		DeletEnemy( CollisionNum );
		return;
	}

}

void cLevelEditor :: SetFastCopyObject( cMVelSprite *nObject )
{
	SetCopyObject( nObject );

	Mouse_command = MOUSE_COMMAND_FASTCOPY;
}

void cLevelEditor :: SetFastCopyObject( void )
{
	SetCopyObject();
	
	Mouse_command = MOUSE_COMMAND_FASTCOPY;
}

void cLevelEditor :: Release_Command( void )
{
	if( Object ) 
	{
		Object = NULL;
	}

	if( Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
	{
		CopyObject = NULL;
	}

	Mouse_command = MOUSE_COMMAND_NOTHING;
}

cMVelSprite *cLevelEditor :: GetCollidingObject( double x, double y )
{
	SDL_Rect cRect = SetRect( (int)x, (int)y, 1, 1 );

	int CollisionNum = -1;

	// Player
	if( RectIntersect( &(const SDL_Rect&)pPlayer->GetRect( SDL_TRUE ), &cRect ) )
	{
		return (cMVelSprite*)pPlayer;
	}

	// Map Objects	
	CollisionNum = pLevel->pLevelData->GetCollidingSpriteNum( &cRect );

	if( CollisionNum >= 0 )
	{
		return pLevel->pLevelData->BasicSprites[CollisionNum];
	}

	// Enemies
	CollisionNum = GetCollidingEnemyNum( &cRect );

	if( CollisionNum >= 0 )
	{
		return (cMVelSprite *)Enemies[CollisionNum];
	}
	
	return NULL;
}

/** @ingroup LE_Input */
void cLevelEditor::EventHandler()
{
	while ( SDL_PollEvent( &event ) )
	{
		// First Let's poll for Mouse button HELD Down
		// You can hold right click to delete tiles constantly
		Uint8 ms = SDL_GetMouseState(NULL,NULL);
		if (ms & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			pLevelEditor->DeleteObject();
		}
		
		if (ms & SDL_BUTTON(SDL_BUTTON_LEFT) && Mouse_command == MOUSE_COMMAND_SELECT_MULTI_TILES)
		{
			pLevelEditor->SetMultiObjects();
		}
		
		UniversalEventHandler(&event);

		switch ( event.type )
		{
			case SDL_QUIT:
			{
				done = 2;
				break;
			}
			case SDL_KEYDOWN:
			{
				// ESCAPE to ESCAPE mouse command or LEVEL MODE
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					
					if (pLevelEditor->Mouse_command == MOUSE_COMMAND_NOTHING && !multiple_objects_selected)
						mode = MODE_GAME;
					else if (multiple_objects_selected)
					{
						MultiObjects.RemoveAllObjects();
						multiple_objects_selected = SDL_FALSE;
					}
					
					
					pLevelEditor->Mouse_command = MOUSE_COMMAND_NOTHING;
					
				}
				// ~ to enter CONSOLE
				else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
				{
					oldmode = mode;
					mode = MODE_CONSOLE;
						
				}
				// F8 to Exit LEVLE MODE
				else if( event.key.keysym.sym == SDLK_F8 )
				{
					
					mode = MODE_GAME;

					pCamera->SetPos( pPlayer->posx - pCamera->x - window_width, 0 );
					
				}
				// IF LCTRL IS HELD
				else if (event.key.keysym.mod & KMOD_LCTRL)
				{
					// LCTRL S to save
					if( event.key.keysym.sym == SAVE_KEY )
					{
						pLevel->Save();	
					}
					// LCTRL+C to copy

					else if( event.key.keysym.sym == COPY_KEY  )
					{
						pLevelEditor->SetCopyObject();
					}
					//LCTRL+V to paste
					else if( event.key.keysym.sym == PASTE_KEY  )
					{
						pLevelEditor->PasteObject();
					}
				}

				// F Key for Fast Copy
				else if (event.key.keysym.sym == FASTCOPY_KEY)
				{
					if (pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY)
						pLevelEditor->Mouse_command = MOUSE_COMMAND_NOTHING;
					else
						pLevelEditor->SetFastCopyObject();
				}
				else if( ( event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_a || 
					event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_d ) ) 
				{
					// level editor mode
					if( pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY && pLevelEditor->CopyObject)
					{
						if( event.key.keysym.sym == SDLK_d) 
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x + pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
							pCamera->Move( pLevelEditor->CopyObject->width, 0 );
						}
						else if( event.key.keysym.sym == SDLK_w )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x, pLevelEditor->CopyObject->posy - pCamera->y - pLevelEditor->CopyObject->height );
							pCamera->Move( 0, - pLevelEditor->CopyObject->height );
						}
						else if( event.key.keysym.sym == SDLK_a )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x - pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
							pCamera->Move( - pLevelEditor->CopyObject->width, 0 );
						}
						else if( event.key.keysym.sym == SDLK_s )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx- pCamera->x , pLevelEditor->CopyObject->posy - pCamera->y + pLevelEditor->CopyObject->height );
							pCamera->Move( 0, pLevelEditor->CopyObject->height );
						}
					}
					else
					{
						// level editor mode
						if( event.key.keysym.sym == SDLK_d) 
						{
							pCamera->Move( 1, 0 );
						}
						else if( event.key.keysym.sym == SDLK_w )
						{
							pCamera->Move( 0, - 1 );
						}
						else if( event.key.keysym.sym == SDLK_a )
						{
							pCamera->Move( - 1, 0 );
						}
						else if( event.key.keysym.sym == SDLK_s )
						{
							pCamera->Move( 0, 1 );
						}						
					}
				}
				break;
			}
				
			case SDL_MOUSEBUTTONDOWN:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL])
					{
						pLevelEditor->SetMultiObjects();
						
						if (multiple_objects_selected && GetCollidingObject(pMouse->posx, pMouse->posy))
							prepareToMove_MultiSelect_Tiles();
						else
						{
							Mouse_command = MOUSE_COMMAND_SELECT_MULTI_TILES;
							multi_mouseXOffset = pMouse->posx;
							multi_mouseYOffset = pMouse->posy;
						}
					}
					else
					{
						if (multiple_objects_selected == SDL_TRUE && (MultiObjects.hasa(GetCollidingObject( pMouse->posx, pMouse->posy )) == SDL_TRUE) )
						{
							// There is an explanation for the explicit checks for SDL_TRUE/SDL_FALSE
							// I was not sure if these values could use the if (varname) since i was unsure of the values
							// it seems safe because SDL_TRUE / SDL_FALSE is 1,0 but i do it explicit to be safer.
							//if ()
							//pLevelEditor->MultiMove();
							
							/* the current process is that */
							prepareToMove_MultiSelect_Tiles();
						}
						else if (!GetCollidingObject(pMouse->posx, pMouse->posy))
						{
							Mouse_command = MOUSE_COMMAND_SELECT_MULTI_TILES;
							multi_mouseXOffset = pMouse->posx;
							multi_mouseYOffset = pMouse->posy;
						}
						else
						{
							// Release any multi-shit
							Release_MultiSelect_Objects();
							pLevelEditor->SetMoveObject();
					
						}
					}
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					//level editor mode
					pLevelEditor->SetFastCopyObject();
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{
					//level editor mode
					//pLevelEditor->DeleteObject();
				}
				break;					
			}
				
			case SDL_MOUSEBUTTONUP:
			{
				if( event.button.button == 1 ) // Left Mouse Button
				{
					if (pMouse->posx == multi_mouseXOffset && pMouse->posy == multi_mouseYOffset && Mouse_command != MOUSE_COMMAND_MOVING_MULTI_TILES)
						Release_MultiSelect_Objects();
					
					pLevelEditor->Release_Command();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
						pLevelEditor->Release_Command();
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{

				}
				break;	
			}
				
			default: break;
		}
	}
}

void cLevelEditor::prepareToMove_MultiSelect_Tiles()
{
	Object = GetCollidingObject( pMouse->posx, pMouse->posy );
	Mouse_command = MOUSE_COMMAND_MOVING_MULTI_TILES;
	
	multi_mouseXOffset = veryfirst_multi_mouseXOffset =  pMouse->posx;
	multi_mouseYOffset = veryfirst_multi_mouseYOffset =  pMouse->posy;
	multi_camXOffset = pCamera->x;
	multi_camYOffset = pCamera->y;
}

SDL_bool cLevelEditor::Release_MultiSelect_Objects()
{
	if (multiple_objects_selected)
	{
		MultiObjects.RemoveAllObjects();
		multiple_objects_selected = SDL_FALSE;
		return SDL_TRUE;
	}
	return SDL_FALSE;
}

void cLevelEditor::HeldKey_fastcopy()
{
	// Lets break this down to HeldKey_fastcopy();
	if( pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY && pLevelEditor->CopyObject)
	{
		if( keys[SDL_SCANCODE_RCTRL] || keys[SDL_SCANCODE_LCTRL] )
		{
			if( keys[SDL_SCANCODE_D])
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x + pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
				pCamera->Move( pLevelEditor->CopyObject->width, 0 );
			}
			else if( keys[SDL_SCANCODE_W] )
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x, pLevelEditor->CopyObject->posy - pCamera->y - pLevelEditor->CopyObject->height );
				pCamera->Move( 0, - pLevelEditor->CopyObject->height );
			}
			else if( keys[SDL_SCANCODE_A] )
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x - pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
				pCamera->Move( - pLevelEditor->CopyObject->width, 0 );
			}
			else if( keys[SDL_SCANCODE_S] )
			{
				usleep(LEVELEDIT_SLEEPWAIT);
				pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx- pCamera->x , pLevelEditor->CopyObject->posy - pCamera->y + pLevelEditor->CopyObject->height );
				pCamera->Move( 0, pLevelEditor->CopyObject->height );
			}
		}
	}
}


void cLevelEditor::MultiCopy( void )
{
	// CALL THIS FUNCTION AFTER YOU'VE SELECTED TILES TO DO THE MULTI-COPY
	// USE AN ARRAY OF TILES WITH THEIR POSITIONS ETC TO COPY
}

void cLevelEditor::HeldKey_movecamera()
{
	if( keys[SDL_SCANCODE_RIGHT] )
	{
		pCamera->Move( 10 * pFramerate->speedfactor, 0 );
	}
	else if( keys[SDL_SCANCODE_LEFT] )
	{
		pCamera->Move( -10 * pFramerate->speedfactor, 0 );
	}
	else if( keys[SDL_SCANCODE_UP] )
	{
		pCamera->Move( 0, -10 * pFramerate->speedfactor );
	}
	else if( keys[SDL_SCANCODE_DOWN] )
	{
		pCamera->Move( 0, 10 * pFramerate->speedfactor );
	}
}

void  cLevelEditor::HeldKey_Handler()
{
	// Fast copy
	HeldKey_fastcopy();
	
	/// Move camera functions
	HeldKey_movecamera();
	
	
	// if at anytime 'e' is pressed. Delete the object the mouse is hovering over.
	if ( keys[SDL_SCANCODE_E] )
	{
		pLevelEditor->DeleteObject();
	}
}
