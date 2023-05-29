#include "Leaf.h"
#include "debug.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->SetState(LEAF_STATE_FLY_AWAY_FROM_BRICK);
	minY = -1;
	minY_2 = -1;
	minX = x;
	maxX = x + 31.0f;
	Fall_Up_start = -1;
	reachMinMax_X = false;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	reachMinMax_X = false;

	if (y <= minY)
	{
		vx = LEAF_FALL_DOWN_SPEED_X;
		vy = LEAF_FALL_DOWN_SPEED_Y;
	}

	if (x >= maxX)
	{
		vx = -LEAF_FALL_DOWN_SPEED_X;
		reachMinMax_X = true;
	}
	if (x < minX)
	{
		vx = LEAF_FALL_DOWN_SPEED_X;
		reachMinMax_X = true;
		//Set vận tốc trục y trong thoáng chốc mà không ảnh hướng đến vận tốc chính
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"Pos Y: %f", y);
}

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (vx > 0)
		animations->Get(ID_ANI_LEAF_RIGHT)->Render(x, y);
	else
		animations->Get(ID_ANI_LEAF_LEFT)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::OnNoCollision(DWORD dt)
{
	if (!reachMinMax_X)
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