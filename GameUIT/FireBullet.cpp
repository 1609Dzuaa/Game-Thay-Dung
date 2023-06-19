#include "FireBullet.h"
#include "Mario.h"
#include "PlayScene.h"

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetStopWatch()) return;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) return;
}

void CFireBullet::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIRE_BULLET_BBOX_WIDTH / 2;
	top = y - FIRE_BULLET_BBOX_HEIGHT / 2;
	right = left + FIRE_BULLET_BBOX_WIDTH;
	bottom = top + FIRE_BULLET_BBOX_HEIGHT;
}

void CFireBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_FIRE_BULLET)->Render(x, y, true);
}