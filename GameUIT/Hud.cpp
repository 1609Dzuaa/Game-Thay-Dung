#include "Hud.h"
#include "Camera.h"
#include "PlayScene.h"
CHud* CHud::__HudInstance = NULL;

CHud* CHud::GetInstance()
{
	if (__HudInstance == NULL)
	{
		float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
		float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() - 16.0f;
		__HudInstance = new CHud(x, y);
	}
	return __HudInstance;
}

void CHud::Update()
{
	//Update vị trí của Hud theo Cam
	this->x= CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
	this->y= CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() - 16.0f;
}

void CHud::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_HUD)->Render(x, y, false);
	RenderCard();
}

void CHud::RenderCard()
{
	CAnimations* animations = CAnimations::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!allowToRenderCard) return;

	HandleCardDrawState();
	if (untouch_0) return;

	int aniId = -1;
	if (mario->GetTypeOfCardCollected() == CARD_STATE_MUSHROOM)
		aniId = ID_ANI_STATIC_CARD_MUSHROOM;
	else if (mario->GetTypeOfCardCollected() == CARD_STATE_STAR)
		aniId = ID_ANI_STATIC_CARD_STAR;
	else if (mario->GetTypeOfCardCollected() == CARD_STATE_FLOWER)
		aniId = ID_ANI_STATIC_CARD_FLOWER;

	if (aniId == -1) return;

	float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() - 75.0f;
	animations->Get(aniId)->Render(x, 372.0f, false);

	//Khởi tạo hiệu ứng chớp chớp cho Card
	if (!isUndrawInitialized)
	{
		untouch_0 = 1;
		untouch_draw_0 = GetTickCount64();
		isUndrawInitialized = 1;
	}
}

void CHud::HandleCardDrawState()
{
	if (GetTickCount64() - untouch_draw_0 >= CARD_UNDRAW_TIME && untouch_0)
	{
		untouch_0 = 0;
		untouch_1 = 1;
		untouch_draw_0 = 0;
		untouch_draw_1 = GetTickCount64();
	} //vẽ
	else if (GetTickCount64() - untouch_draw_1 >= CARD_UNDRAW_TIME && untouch_1)
	{
		untouch_0 = 1;
		untouch_1 = 0;
		untouch_draw_0 = GetTickCount64();
		untouch_draw_1 = 0;
	}
}