#include "MarioWorld.h"
//#include "PlayScene.h" Get Mario's level, coin, HP, Score, to Update World Hud
#include "GameObject.h"
#include "Entrance.h"
#include "Game.h"
#include "debug.h"

void CMarioWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (HasCollidedWithEntrance)
		HandlePositionWithEntrance();
	
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"MarX, EntrX: %f, %f", x, Entrance_Position.x);
}

void CMarioWorld::HandlePositionWithEntrance()
{
	if (vx > 0)
	{
		if (x - Entrance_Position.x >= 0)
		{
			x = Entrance_Position.x;
			vx = 0;
			HasCollidedWithEntrance = false;
			isMoving = false;
		}
	}
	else if (vx < 0)
	{
		if (x - Entrance_Position.x <= 0)
		{
			x = Entrance_Position.x;
			vx = 0;
			HasCollidedWithEntrance = false;
			isMoving = false;
		}
	}
	//Mục đích đưa nhân vật vào giữa Entrance dù Collided
}

void CMarioWorld::Render() 
{
	CAnimations* animations = CAnimations::GetInstance();
	
	int aniId = ID_ANI_MARIO_SMALL_AT_WORLD;
	animations->Get(aniId)->Render(x, y, false);
}

void CMarioWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	top = y - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	right = left + MARIO_WORLD_MAP_BBOX_HEIGHT;
	bottom = top + MARIO_WORLD_MAP_BBOX_HEIGHT;

}

void CMarioWorld::OnNoCollision(DWORD dt) 
{
	x += vx * dt;
	y += vy * dt;
}

void CMarioWorld::OnCollisionWith(LPCOLLISIONEVENT e) 
{
	if (e->obj->IsBlocking())
	{
		vx = 0;
		vy = 0;
	}
	if (dynamic_cast<CEntrance*>(e->obj))
		OnCollisionWithEntrance(e);
}

void CMarioWorld::OnCollisionWithEntrance(LPCOLLISIONEVENT e) 
{
	//'ll do this by myself ^.^
	CEntrance* entr = dynamic_cast<CEntrance*>(e->obj);
	Entrance_Type = entr->GetType();
	Entrance_Position.x = e->obj->GetX();
	Entrance_Position.y = e->obj->GetY();
	HasCollidedWithEntrance = true;
}

void CMarioWorld::SetState(int state) 
{
	switch (state)
	{
	case MARIO_WORLD_STATE_MOVE_RIGHT:
		isMoving = true;
		vx = 0.1f;
		vy = 0; //Chỉ vận tốc di chuyển theo 1 trục
		break;

	case MARIO_WORLD_STATE_MOVE_LEFT:
		isMoving = true;
		vx = -0.1f;
		vy = 0;
		break;

	case MARIO_WORLD_STATE_MOVE_UP:
		isMoving = true;
		vy = -0.1f;
		vx = 0;
		break;

	case MARIO_WORLD_STATE_MOVE_DOWN:
		isMoving = true;
		vy = 0.1f;
		vx = 0;
		break;

	case MARIO_WORLD_STATE_ENTER_ENTRANCE:
		if (!IsPassedThisEntrance(Entrance_Type))
			isAllowToPlayThatEntrance = true;
		break;
	}
}

bool CMarioWorld::IsPassedThisEntrance(int entr_type)
{
	for (int i = 0; i < EntranceHasPassed.size(); i++)
		if (entr_type == EntranceHasPassed[i])
			return true;
	return false;
}