

#include "include\Globals.h"



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
	HoveredObject = SetRect( 0, 0, 0, 0 );

	if( Mouse_command == MOUSE_COMMAND_NOTHING || Mouse_command == MOUSE_COMMAND_FASTCOPY ) 
	{
		cMVelSprite *Collision_Object = GetCollidingObject( pMouse->posx, pMouse->posy );

		if( Collision_Object ) 
		{
			HoveredObject = Collision_Object->GetRect( 1 );
		}
	}
	else if( Mouse_command == MOUSE_COMMAND_MOVING )
	{
		Object->SetPos( pMouse->posx + Mouse_w + pCamera->x, pMouse->posy + Mouse_h + pCamera->y );
		
		Object->Startposx = pMouse->posx + Mouse_w + pCamera->x;
		Object->Startposy = pMouse->posy + Mouse_h + pCamera->y;
	}

}

void cLevelEditor :: Draw( SDL_Surface *target )
{
	Uint32 Color;

	if( Mouse_command != MOUSE_COMMAND_FASTCOPY ) 
	{
		Color = SDL_MapRGB( target->format, 255, 255, 255 ); // White
	}
	else
	{
		Color = SDL_MapRGB( target->format, 120, 120, 255 ); // Blue
	}
	

	// The HoveredObject Shadow
	sge_RectAlpha( Screen ,HoveredObject.x + 1, HoveredObject.y + 1, HoveredObject.x + 1 + HoveredObject.w, 
					HoveredObject.y + 1 + HoveredObject.h, 0, 0, 0, 128 );

	// Draws an non Filled Rect with the HoveredObject rect size
	sge_Rect( Screen ,HoveredObject.x, HoveredObject.y, HoveredObject.x + HoveredObject.w, 
					HoveredObject.y + HoveredObject.h, Color );

	if( CopyObject && !FullRectIntersect( &HoveredObject, &CopyObject->rect ) && Mouse_command != MOUSE_COMMAND_FASTCOPY ) 
	{
		// The CopyObject Shadow
		sge_RectAlpha( Screen ,(int)( CopyObject->posx - pCamera->x + 1 ), (int)( CopyObject->posy - pCamera->y + 1 ), (int)( CopyObject->posx - pCamera->x + 1 + CopyObject->width ), 
						(int)( CopyObject->posy - pCamera->y + 1 + CopyObject->height ), 0, 0, 0, 64 );

		// Draws an non Filled Rect with the CopyObject rect size
		sge_RectAlpha( Screen ,(int)( CopyObject->posx - pCamera->x ), (int)( CopyObject->posy - pCamera->y ), (int)( CopyObject->posx - pCamera->x + CopyObject->width ), 
						(int)( CopyObject->posy - pCamera->y + CopyObject->height ), 120, 255, 120, 192  );	
	}
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

void cLevelEditor :: SetMoveObject( cMVelSprite *nObject )
{
	if( !nObject ) 
	{
		return;
	}

	Mouse_w = (int)(nObject->posx -= pMouse->posx + pCamera->x);
	Mouse_h = (int)(nObject->posy -= pMouse->posy + pCamera->y);
	
	Object = nObject;

	Mouse_command = MOUSE_COMMAND_MOVING;
}

void cLevelEditor :: SetMoveObject( void )
{
	// Everything can be moved
	SetMoveObject( GetCollidingObject( pMouse->posx, pMouse->posy )  );
}

void cLevelEditor :: SpecialPasteObject()
{
	if (!lastCopiedObject)
	{
		PasteObject();
	}
	else
	{
		SpecialPasteObject(lastCopiedObject->posx+lastCopiedObject->width, lastCopiedObject->posy);
	}
}

void cLevelEditor :: SpecialPasteObject( double x, double y )
{
	if( !CopyObject ) 
	{
		return;
	}


	// Add the New Object
	if( CopyObject->type == SPRITE_TYPE_MASSIVE || CopyObject->type == SPRITE_TYPE_PASSIVE || CopyObject->type == SPRITE_TYPE_HALFMASSIVE) 
	{
		// Create the new Sprite
		cMVelSprite *new_Object = new cMVelSprite( CopyObject->srcimage, x, y );

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
	if( RectIntersect( &pPlayer->GetRect( 1 ), &cRect ) ) 
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

void leveleditor_ehandler()
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

		switch ( event.type )
		{
		case SDL_QUIT:
			{
				done = 2;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					// universal!
					mode = MODE_GAME;
				}
				else if ( event.key.keysym.sym == SDLK_BACKQUOTE )
				{
					oldmode = mode;
					mode = MODE_CONSOLE;
						
				}
				else if( event.key.keysym.sym == SDLK_F8 )
				{
					
					mode = MODE_GAME;

					//pCamera->SetPos( pPlayer->posx - pCamera->x - Screen->w, 0 );
					
				}
				else if( event.key.keysym.sym == SDLK_s && ( event.key.keysym.mod & KMOD_LCTRL ) ) 
				{
					//level edit mode
					pLevel->Save();
					
				}
				else if( event.key.keysym.sym == SDLK_c && ( event.key.keysym.mod & KMOD_LCTRL ) ) 
				{
					pLevelEditor->SetCopyObject();
				}
				else if( event.key.keysym.sym == SDLK_v && ( event.key.keysym.mod & KMOD_LCTRL ) ) 
				{
					pLevelEditor->PasteObject();
				}
				/* New Code Special Paste 12/30/12 */
				else if( event.key.keysym.sym == SDLK_v && ( event.key.keysym.mod & KMOD_RCTRL ) ) 
				{
					pLevelEditor->SpecialPasteObject();
				}
				else if( ( event.key.keysym.sym == SDLK_KP6 || event.key.keysym.sym == SDLK_KP8 || 
					event.key.keysym.sym == SDLK_KP4 || event.key.keysym.sym == SDLK_KP2 ) ) 
				{
					// level editor mode
					if( pLevelEditor->Mouse_command == MOUSE_COMMAND_FASTCOPY && pLevelEditor->CopyObject)
					{
						if( event.key.keysym.sym == SDLK_KP6 ) 
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x + pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
							pCamera->Move( pLevelEditor->CopyObject->width, 0 );
						}
						else if( event.key.keysym.sym == SDLK_KP8 )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x, pLevelEditor->CopyObject->posy - pCamera->y - pLevelEditor->CopyObject->height );
							pCamera->Move( 0, - pLevelEditor->CopyObject->height );
						}
						else if( event.key.keysym.sym == SDLK_KP4 )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx - pCamera->x - pLevelEditor->CopyObject->width, pLevelEditor->CopyObject->posy - pCamera->y );
							pCamera->Move( - pLevelEditor->CopyObject->width, 0 );
						}
						else if( event.key.keysym.sym == SDLK_KP2 )
						{
							pLevelEditor->PasteObject( pLevelEditor->CopyObject->posx- pCamera->x , pLevelEditor->CopyObject->posy - pCamera->y + pLevelEditor->CopyObject->height );
							pCamera->Move( 0, pLevelEditor->CopyObject->height );
						}
					}
					else
					{
						// level editor mode
						if( event.key.keysym.sym == SDLK_KP6) 
						{
							pCamera->Move( 1, 0 );
						}
						else if( event.key.keysym.sym == SDLK_KP8 )
						{
							pCamera->Move( 0, - 1 );
						}
						else if( event.key.keysym.sym == SDLK_KP4 )
						{
							pCamera->Move( - 1, 0 );
						}
						else if( event.key.keysym.sym == SDLK_KP2 )
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
					//level editor mode
						pLevelEditor->SetMoveObject();
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
					//level editor mode
					pLevelEditor->Release_Command();
				}
				else if( event.button.button == 2 ) // Middle Mouse Button
				{
					//level editor mode
						pLevelEditor->Release_Command();
					//level editor mode
				}
				else if( event.button.button == 3 ) // Right Mouse Button
				{

				}
				break;	
			}
		default:
			break;
		}
	}
}