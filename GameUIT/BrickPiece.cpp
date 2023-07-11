#include "BrickPiece.h"
#include "Camera.h"
#include "PlayScene.h"
#include "DataBinding.h"

void CBrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CDataBindings::GetInstance()->IsStopWatch) return;

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
	if (IsWaiting && IsWaitable) return;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK_PIECE)->Render(x, y, true);
}