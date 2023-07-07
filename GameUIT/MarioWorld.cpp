#include "MarioWorld.h"
#include "GameObject.h"
#include "Entrance.h"
#include "Game.h"
#include "debug.h"

void CWorldMapPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CWorldMapPlayer::Render() 
{
	CAnimations* animations = CAnimations::GetInstance();
	
	int aniId = ID_ANI_MARIO_SMALL_AT_WORLD;
	animations->Get(aniId)->Render(x, y, false);
}

void CWorldMapPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	top = y - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	right = left + MARIO_WORLD_MAP_BBOX_HEIGHT;
	bottom = top + MARIO_WORLD_MAP_BBOX_HEIGHT;

}

void CWorldMapPlayer::OnNoCollision(DWORD dt) 
{
	x += vx * dt;
	y += vy * dt;
}

void CWorldMapPlayer::OnCollisionWith(LPCOLLISIONEVENT e) 
{
	if (e->obj->IsBlocking()) {
		vx = 0;
		vy = 0;
	}
	if (dynamic_cast<CEntrance*>(e->obj))
		OnCollisionWithEntrance(e);
}

void CWorldMapPlayer::OnCollisionWithEntrance(LPCOLLISIONEVENT e) 
{
	//'ll do this by myself ^.^

}

void CWorldMapPlayer::SetState(int state) 
{
	
}