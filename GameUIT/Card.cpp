#include "Card.h"
#include "Mushroom.h"
#include "Camera.h"
#include "EffectScore.h"
#include "debug.h"

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CCamera::GetInstance()->isViewable(this)) return;
	//Card bị Collect, đi hết màn hình rồi mới hiện Text
	if (state == CARD_STATE_STATIC) return;

	if (y <= 100.0f && state == CARD_STATE_BE_COLLECTED)
		this->Delete();	//Xoá card này đi

	if (state != CARD_STATE_BE_COLLECTED && state != CARD_STATE_STATIC)
	{
		if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD && isMushroom)
			SetState(CARD_STATE_STAR);
		else if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD && isStar)
			SetState(CARD_STATE_FLOWER);
		else if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD && isFlower)
			SetState(CARD_STATE_MUSHROOM);
	}

	DebugOutTitle(L"St, y: %d, %f", state, y);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCard::Render()
{
	//if (!CCamera::GetInstance()->isViewable(this)) return;

	int aniID = -1;
	if (state == CARD_STATE_MUSHROOM)
		aniID = ID_ANI_CARD_MUSHROOM;
	else if (state == CARD_STATE_STAR)
		aniID = ID_ANI_CARD_STAR;
	else if(state == CARD_STATE_FLOWER)
		aniID = ID_ANI_CARD_FLOWER;
	else if (state == CARD_STATE_BE_COLLECTED)
	{
		if (isMushroom)
			aniID = ID_ANI_BLACK_CARD_MUSHROOM;
		else if (isStar)
			aniID = ID_ANI_BLACK_CARD_STAR;
		else
			aniID = ID_ANI_BLACK_CARD_FLOWER;
	}
	else
	{
		if (isMushroom)
			aniID = ID_ANI_STATIC_CARD_MUSHROOM;
		else if (isStar)
			aniID = ID_ANI_STATIC_CARD_STAR;
		else
			aniID = ID_ANI_STATIC_CARD_FLOWER;
	}
	CAnimations* animations = CAnimations::GetInstance();
	if (aniID != -1 && state != CARD_STATE_STATIC)
		animations->Get(aniID)->Render(x, y, false);
	else if (state == CARD_STATE_STATIC)
		animations->Get(aniID)->Render(x, GOT_A_CARD_Y - 3.0f, false);
}

void CCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - CARD_BBOX_WIDTH / 2;
	t = y - CARD_BBOX_HEIGHT / 2;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_HEIGHT;
}

void CCard::SetState(int state)
{
	switch (state)
	{
	case CARD_STATE_MUSHROOM:
		switch_card_start = GetTickCount64();
		isMushroom = 1;
		isStar = 0;
		isFlower = 0;
		break;

	case CARD_STATE_STAR:
		switch_card_start = GetTickCount64();
		isMushroom = 0;
		isStar = 1;
		isFlower = 0;
		break;

	case CARD_STATE_FLOWER:
		switch_card_start = GetTickCount64();
		isMushroom = 0;
		isStar = 0;
		isFlower = 1;
		break;

	case CARD_STATE_BE_COLLECTED:
		vy = -CARD_FLY_SPEED;
		break;
	}

	CGameObject::SetState(state);
}

void CCard::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}