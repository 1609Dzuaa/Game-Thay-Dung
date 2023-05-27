#include "Leaf.h"
#include "debug.h"

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (y <= minY)
	{
		vy = LEAF_FALL_SPEED_Y;
		vx = LEAF_FALL_SPEED_X;
	}

	if (x > maxX)
		vx = -LEAF_FALL_SPEED_X;
	if (x < minX)
		vx = LEAF_FALL_SPEED_X;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"Velo: %f", vy);
}

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (vx > 0)
		animations->Get(ID_ANI_LEAF_RIGHT)->Render(x, y);
	else
		animations->Get(ID_ANI_LEAF_LEFT)->Render(x, y);
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}