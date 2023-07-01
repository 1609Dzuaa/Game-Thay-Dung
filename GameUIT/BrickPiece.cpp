#include "BrickPiece.h"
#include "Camera.h"
#include "PlayScene.h"

void CBrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetStopWatch()) return;

	vy += ay * dt;
	//if(!CCamera::GetInstance()->isViewable(this)) xoá đi nếu đi khỏi tầm
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBrickPiece::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CBrickPiece::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK_PIECE)->Render(x, y, true);
}