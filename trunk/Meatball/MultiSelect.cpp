#include "MultiSelect.h"
#include "leveleditor.h"
#include "player.h"
#include "level.h"
#include "Camera.h"
#include "enemy.h"

extern cPlayer *pPlayer;
extern cLevel *pLevel;
extern cCamera *pCamera;

using namespace std;

void cMultiSelect :: DrawTileOutlines(SDL_Renderer *renderer)
{
	//HoveredObjectRect = SetRect( 0, 0, 0, 0 );
	
	// This is code to get a RECT from our sprite Tile
	if (multiple_objects_selected)
	{
		for (unsigned int i=0; i < OM.objcount; i++)
		{
			// Draw a rect around each individual object
			
			// get its rect
			cMVelSprite *ptr = OM.objects[i];
			SDL_Rect orect = ptr->GetRect(SDL_TRUE);
			
			cLevelEditor::OutlineObject(renderer, TILE_OUTLINE_COLOR, &orect);
		}
	}
}

void cMultiSelect::DrawRect(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	FillRectAlpha(renderer, &rect, RECT_COLOR );
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void cMultiSelect::DoRect()
{
	double diff_X = pMouse->posx - rectX_origin;
	double diff_Y = pMouse->posy - rectY_origin;
	
	if (diff_X < 0)
	{
		rect.x = (int)pMouse->posx;
		// new width is from X to original X
		rect.w = (int)rectX_origin - rect.x;
	}
	else
	{
		rect.x = (int)rectX_origin;
		rect.w = (int)diff_X;
	}
	
	if (diff_Y < 0)
	{
		rect.y = (int)pMouse->posy;
		// new height is from Y to original Y
		rect.h = (int)rectY_origin - rect.y;
	}
	else
	{
		rect.y = (int)rectY_origin;
		rect.h = (int)diff_Y;
	}
}

void cMultiSelect::NewMoveObjects()
{
	PasteObjects();
	PrepareToMove();
}

void cMultiSelect::CopyObjects()
{
	
}

void cMultiSelect::DeleteObjects()
{
	// Only Map Objects and Enemies can be deleted
	
	
	int index;
	
	for (unsigned int i=0; i < OM.objcount; i++)
	{
		// Map Objects
		index = pLevel->pLevelData_Layer1->BasicSprites.hasa(OM.objects[i]);
		if (index >= 0)
		{
			pLevel->pLevelData_Layer1->DeleteSprite( index );
			continue;
		}

		index = Enemies.hasa((cEnemy*)OM.objects[i]);
		if (index >= 0)
		{
			DeletEnemy( index);
			continue;
		}
	}
	
	OM.~ObjectManager();
	multiple_objects_selected = SDL_FALSE;

}

void cMultiSelect::PasteObjects()
{
	SDL_Point leftmost,topmost;
	
	if (OM.objects && OM.objcount > 0)
	{
		leftmost.x = topmost.x = OM.objects[0]->posx;
		leftmost.y = topmost.y = OM.objects[0]->posy;
	}
	
	if (OM.objcount > 1)
	{
		for (unsigned int i=1; i < OM.objcount; i++)
		{
			int x,y;
			x = OM.objects[i]->posx;
			y = OM.objects[i]->posy;
			
			if (x <= leftmost.x)
			{
				if (x == leftmost.x)
				{
					if (y < leftmost.y)
						leftmost.y = y;
				}
				else
				{
					leftmost.x = x;
					leftmost.y = y;
				}
			}
			
			if (topmost.y >= y)
			{
				if (topmost.y == y)
				{
					if (x < topmost.x)
						topmost.x = x;
				}
				else
				{
					topmost.y = y;
					topmost.x = x;
				}
			}
		}
	}
	
	// get offsets
	int x_offset, y_offset;
	SDL_Point *selected;
	if (topmost.x > leftmost.x)
	{
		x_offset = topmost.x-leftmost.x;
	}
	else
		x_offset = leftmost.x - topmost.x;
	
	if (topmost.y > leftmost.y)
	{
		y_offset = topmost.y-leftmost.y;
	}
	else
		y_offset = leftmost.y - topmost.y;
	
	if (x_offset > y_offset)
		selected = &leftmost;
	else selected = &topmost;
	
	for (unsigned int i=0; i < OM.objcount; i++)
	{
		// Add the New Object
		if( OM.objects[i]->type == SPRITE_TYPE_MASSIVE || OM.objects[i]->type == SPRITE_TYPE_PASSIVE || OM.objects[i]->type == SPRITE_TYPE_HALFMASSIVE)
		{
			// Create the new Sprite
			
			cMVelSprite *new_Object = new cMVelSprite( OM.objects[i]->srcimage, floor((pMouse->posx-selected->x) + (pCamera->x) + OM.objects[i]->posx),
													  floor( (pMouse->posy - selected->y) + (pCamera->y) + OM.objects[i]->posy) );
			
			new_Object->type = OM.objects[i]->type;
			
			pLevel->pLevelData_Layer1->AddSprite( new_Object );
			OM.objects[i] = new_Object;
		}
		else if( OM.objects[i]->type == SPRITE_TYPE_ENEMY )
		{
			cEnemy *pEnemy = (cEnemy *)OM.objects[i];
			
			AddEnemy(floor((pMouse->posy - mouseYOffset) + (pCamera->y-camYOffset) + OM.objects[i]->posy), floor((pMouse->posy - mouseYOffset) + (pCamera->y-camYOffset) + OM.objects[i]->posy), pEnemy->Enemy_type );
			
			OM.objects[i] = pEnemy;
		}
	}
}

void cMultiSelect :: SetObjects( void )
{
	// Only Map and Enemy Objects can be Copied (FOR NOW)
	// SetObject( GetCollidingObject( pMouse->posx, pMouse->posy )  );
	GetAllCollidingObjects(&rect, &OM);
	
	multiple_objects_selected = (OM.objcount > 0) ? SDL_TRUE:SDL_FALSE;
}
void cMultiSelect :: SetObject( cMVelSprite *nObject)
{
	if( !nObject )
	{
		return;
	}
	
	int index = OM.hasa(nObject);
	if ( index < 0 )
	{
		OM.add(nObject);
		multiple_objects_selected = SDL_TRUE;
	}
	else
	{
		OM.Remove(index);
	}
	
	
}

void cMultiSelect::Move(void)
{
	for (unsigned int i=0; i < OM.objcount; i++)
	{
		OM.objects[i]->SetPos( floor((pMouse->posx-mouseXOffset) + (pCamera->x-camXOffset) + OM.objects[i]->posx), floor((pMouse->posy - mouseYOffset) + (pCamera->y-camYOffset) + OM.objects[i]->posy) );
		
		OM.objects[i]->Startposx = OM.objects[i]->posx;
		OM.objects[i]->Startposy = OM.objects[i]->posy;
	}
	
	mouseXOffset = pMouse->posx;
	mouseYOffset = pMouse->posy;
	camXOffset = pCamera->x;
	camYOffset = pCamera->y;
}
SDL_bool cMultiSelect :: GetAllCollidingObjects( SDL_Rect *cRect, ObjectManager<cMVelSprite> *obj_man )
{
	// These variables are going to hold the data we need :)
	SDL_bool were_objects_found = SDL_FALSE;
	
	//obj_man->RemoveAllObjects();
	
	// Player
	if( RectIntersect( &(const SDL_Rect&)pPlayer->GetRect( SDL_TRUE ), cRect ) )
	{
		if (obj_man->hasa((cMVelSprite *)pPlayer) < 0)
		{
			obj_man->add((cMVelSprite*)pPlayer);
			were_objects_found = SDL_TRUE;
		}
	}
	
	// Map Objects
	if (pLevel->pLevelData_Layer1->GetAllCollidingSpriteNum( cRect, obj_man ))
		were_objects_found = SDL_TRUE;
	
	
	// Do ENEMIES LATER
	
	// Enemies
	/*CollisionNum = GetCollidingEnemyNum( cRect );
	 
	 if( CollisionNum >= 0 )
	 {
	 return (cMVelSprite *)Enemies[CollisionNum];
	 }*/
	
	//return NULL;
	
	return were_objects_found;
}
void cMultiSelect::InitTiles(SDL_bool release/* = SDL_FALSE */)
{
	cLevelEditor::Mouse_command = MOUSE_COMMAND_SELECT_MULTISELECT_TILES;
	
	mouseXOffset = pMouse->posx;
	mouseYOffset = pMouse->posy;
	
	rectX_origin = pMouse->posx;
	rectY_origin = pMouse->posy;
	
	rect.x = (int)pMouse->posx;
	rect.y = (int)pMouse->posy;
	
	rect.w = 0;
	rect.h = 0;
	
	// Release last time's old tiles
	if (release)
		Release();
}

void cMultiSelect :: Prepare()
{
	//mouseXOffset = veryfirst_mouseXOffset =  pMouse->posx;
	//mouseYOffset = veryfirst_mouseYOffset =  pMouse->posy;
	//camXOffset = pCamera->x;
	//camYOffset = pCamera->y;
}

void cMultiSelect::PrepareToMove()
{
	//Object = GetCollidingObject( pMouse->posx, pMouse->posy );
	cLevelEditor::Mouse_command = MOUSE_COMMAND_MOVING_MULTISELECT_TILES;
	
	mouseXOffset = veryfirst_mouseXOffset =  pMouse->posx;
	mouseYOffset = veryfirst_mouseYOffset =  pMouse->posy;
	camXOffset = pCamera->x;
	camYOffset = pCamera->y;
}

SDL_bool cMultiSelect::Release()
{
	OM.RemoveAllObjects();
	
	if (multiple_objects_selected)
	{
		//OM.RemoveAllObjects();
		multiple_objects_selected = SDL_FALSE;
		return SDL_TRUE;
	}
	return SDL_FALSE;
}