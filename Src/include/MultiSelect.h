#pragma once

#include "Globals.h"
#include "sprite.h"
#include "ObjectManager.h"

class cLevelEditor;

#define TILE_OUTLINE_COLOR 0xff00ff00
#define RECT_COLOR 0x4000f080

class cMultiSelect
{
public:
	// This next section is for the MultiSelect Tiles
	/////////////////////////////////////////////////////////
	void DrawTileOutlines(SDL_Renderer *renderer);
	
	// Could be worded better 
	void NewMoveObjects();
	
	void CopyObjects();
	void PasteObjects();
	void DeleteObjects();
	/// Add to OM array
	void AddObject( cMVelSprite *nObject);
	void AddObjects( SDL_bool erase_current_rect );
	// MultiSelect_ move
	void Move(void);
	void DoRect();
	// the actual MultiSelect_-copy process
	//void MultiSelect_Copy( void );
	
	void DrawRect(SDL_Renderer *renderer);
	
	void InitTiles(SDL_bool release = SDL_FALSE);
	void PrepareToMove();
	void Prepare();
	
	SDL_bool GetAllCollidingObjects( SDL_Rect *crect, ObjectManager<cMVelSprite> &obj_man, SDL_bool erase_current_rect = SDL_TRUE );
	
	/// Releases any MultiSelect_ selected object
	/// @returns SDL_TRUE if there were objects to remove, SDL_FALSE if there wasnt
	SDL_bool Release();
	
	// this is true if we have MultiSelect_-selected > 1 tile
	// this is because if we only MultiSelect_-select 1 tile. we might as well just move in 1 tile mode
	SDL_bool multiple_objects_selected;
	
	/// these very first ones are because I want to show the grid around selected tiles all the way until the point that the tiles actually start moving
	// So I take these first offsets of one of the tiles I multiselect (the first one), I show the grid until there is a change from these coordinates
	double veryfirst_mouseXOffset, veryfirst_mouseYOffset;
	
	double mouseXOffset, mouseYOffset;
	double camXOffset, camYOffset;
	
	SDL_Rect rect;
	//
	double rectX_origin, rectY_origin;
	//
	SDL_bool rect_XDirection;
	SDL_bool rect_YDirection;
	
	// Object MAnager for MultiSelect_-object
	ObjectManager<cMVelSprite> OM;
};