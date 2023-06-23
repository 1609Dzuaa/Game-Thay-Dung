#include "Coin.h"
#include "PlayScene.h"
#include "debug.h"
#include "Camera.h"

void CCoin::Render()
{
	if (!CCamera::GetInstance()->isViewable(this)) return;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y, false);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->type == COIN_TYPE_FLY)
		HandleFlyingCoin();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::HandleFlyingCoin()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (y <= minY)
	{
		vy = COIN_FLY_SPEED;
	}
	if (y > currentY)
	{
		y = currentY;
		vy = 0;
		this->Delete();
		mario->SpawnScore(this); //only give 100
	}
}

void CCoin::OnNoCollision(DWORD dt)
{
	if (this->type == COIN_TYPE_FLY)
		y += vy * dt;
}