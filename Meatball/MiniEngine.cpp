#include "MiniEngine.h"
#include "level.h"
#include "Camera.h"
#include "player.h"
#include "enemy.h"
#include "effects.h"
#include "bullet.h"

extern cFramerate *pFramerate;
extern cMouseCursor *pMouse;
extern cCamera *pCamera;
extern SDL_Renderer *Renderer;
extern cLevel *pLevel;
extern SDL_bool paused;
extern cPlayer *pPlayer;

void MiniEngine::Do()
{
	// Might have to change the whole ONLY checking input while !paused thing...
	if ( !paused )
	{
		Input();
		Update();
	}
	Draw();
}

/// Does general things
void MiniEngine::PreUpdate( void )
{
	pFramerate->SetSpeedFactor(); // Update
	
	pCamera->Update();
	
	pMouse->Update(Renderer);
}

void MiniEngine::Update()
{
	PreUpdate();
	PostUpdate();
}

void MiniEngine::PostUpdate()
{
	
	
}

void MiniEngine::PreDraw()
{
	SDL_RenderClear(Renderer);
	
	pLevel->Draw();
}
void MiniEngine::PostDraw()
{
	FramerateDraw( Renderer );
	
	SDL_RenderPresent(Renderer);
}
void MiniEngine::Draw( void )
{
	PreDraw();
    
	DrawBullets(Renderer);
	DrawParticleEmitter(Renderer);
	
	DrawEnemies(Renderer);
	
	pPlayer->Draw( Renderer );
	
	
	PostDraw();
}