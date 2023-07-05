#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"
#include "debug.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->SetState(LEAF_STATE_FLY_AWAY_FROM_BRICK);
	minY = -1;
	minX = x;
	maxX = x + 31.0f;
	Fall_Up_start = -1;
	reachMinOrMax_X = false;
	IsWaitable = true;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	reachMinOrMax_X = false;

	if (y <= minY)
	{
		vx = LEAF_FALL_DOWN_SPEED_X;
		vy = LEAF_FALL_DOWN_SPEED_Y;
	}

	if (x >= maxX)
	{
		vx = -LEAF_FALL_DOWN_SPEED_X;
		reachMinOrMax_X = true;
	}
	if (x < minX)
	{
		vx = LEAF_FALL_DOWN_SPEED_X;
		reachMinOrMax_X = true;
		//Set vận tốc trục y trong thoáng chốc mà không ảnh hướng đến vận tốc chính
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"Pos Y: %f", y);
}

void CLeaf::Render()
{
	if (IsWaiting && IsWaitable) return;

	CAnimations* animations = CAnimations::GetInstance();
	if (vx > 0)
		animations->Get(ID_ANI_LEAF_RIGHT)->Render(x, y, true);
	else
		animations->Get(ID_ANI_LEAF_LEFT)->Render(x, y, true);
}

void CLeaf::OnNoCollision(DWORD dt)
{
	if (!reachMinOrMax_X)
	{
		x += vx * dt;
		y += vy * dt;
	}
	else
	{
		x += vx * dt;
		y += vy * dt;
	}
}

void CLeaf::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	/*if (dynamic_cast<CMario*>(e->obj))
	{
		this->Delete();
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		{
			this->isEvolveForward = true;
			this->SetState(MARIO_STATE_EVOLVING);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_BIG)
		{
			this->isEvolveForward = true;
			this->SetState(MARIO_STATE_EVOLVING);
			SpawnEffect(e, leaf, EFF_COL_TYPE_SMOKE_EVOLVE);
		}
		SpawnScore(leaf);
	}*/
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::SetState(int state)
{
	switch (state)
	{
	case LEAF_STATE_FALL_DOWN:
		vx = LEAF_FALL_DOWN_SPEED_X;
		vy = LEAF_FALL_DOWN_SPEED_Y;
		ay = LEAF_GRAVITY;
		break;

	case LEAF_STATE_FALL_UP:
		Fall_Up_start = GetTickCount64();
		break;

	case LEAF_STATE_FLY_AWAY_FROM_BRICK:
		vy = -LEAF_FLY_SPEED;
		break;
	}

	CGameObject::SetState(state);
}