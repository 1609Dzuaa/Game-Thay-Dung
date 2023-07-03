#include "Card.h"
#include "Mushroom.h"
#include "Camera.h"
#include "debug.h"

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CCamera::GetInstance()->isViewable(this)) return;

	if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD && isMushroom)
		SetState(CARD_STATE_STAR);
	else if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD && isStar)
		SetState(CARD_STATE_FLOWER);
	else if (GetTickCount64() - switch_card_start > TIME_SWITCH_CARD && isFlower)
		SetState(CARD_STATE_MUSHROOM);

	//DebugOutTitle(L"St: %d", state);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCard::Render()
{
	if (!CCamera::GetInstance()->isViewable(this)) return;

	int aniID = -1;
	if (state == CARD_STATE_MUSHROOM)
		aniID = ID_ANI_RED_MUSHROOM;
	else if (state == CARD_STATE_STAR)
		aniID = ID_ANI_CARD_STAR;
	else if(state == CARD_STATE_FLOWER)
		aniID = ID_ANI_CARD_FLOWER;
	CAnimations* animations = CAnimations::GetInstance();
	if (aniID != -1)
		animations->Get(aniID)->Render(x, y, false);
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
	}

	CGameObject::SetState(state);
}