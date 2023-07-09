#include "BlackScreen.h"
#include "Camera.h"
#include "Textures.h"
#include "PlayScene.h"
#include "WorldPlayScene.h"
#include "debug.h"

CBlackScreen* CBlackScreen::__BlackScrInstance = nullptr;

CBlackScreen* CBlackScreen::GetInstance()
{
	if (__BlackScrInstance == nullptr)
	{
		CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
		if (current_scene->GetID() != ID_WORLD)
		{
			float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
			float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() / 2;
			__BlackScrInstance = new CBlackScreen(x, y);
		}
		else
		{
			float x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2);
			float y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2);
			__BlackScrInstance = new CBlackScreen(x, y);
		}
		DebugOut(L"Create Black Screen Successfully\n");
	}
	return __BlackScrInstance;
}

void CBlackScreen::Update()
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
	//chỉ khi đang ở PlayScene thì mới Update vị trí BlackScreen, 0 thì thôi
	if (current_scene->GetID() != ID_WORLD)
	{
		float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
		float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() / 2;
		this->x = x;
		this->y = y;
	}

	if (state == BLACK_SCR_EFF_STATE_DRAW_FROM_0)
	{
		Alpha += 0.02f;
		if (Alpha >= 1.0f)
		{
			Alpha = 1.0f;
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->SetIsWaitingForTrans(0);
			state = BLACK_SCR_EFF_STATE_DRAW_FROM_1;
		}
	}
	else if (state == BLACK_SCR_EFF_STATE_DRAW_FROM_1)
	{
		Alpha -= 0.02f;
		if (Alpha <= 0.0f)
		{
			Alpha = 0.0f;
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->SetIsWaitingForTrans(0);
			state = BLACK_SCR_EFF_STATE_STOP_DRAW;
		}
	}

	//DebugOut(L"St, Al: %d, %f\n", state, Alpha);
}

void CBlackScreen::Render()
{
	//Lấy ý tưởng từ hàm RenderBoundingBox trong Framework, vẽ cái screen mờ đi theo độ Alpha

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_BLACK_SCREEN);

	int l, t, r, b;
	l = t = r = b = 0;
	//Done Warning here!
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	if (current_scene->GetID() != ID_WORLD)
	{
		l = static_cast<int>(x - CCamera::GetInstance()->GetCamPos().x);
		t = static_cast<int>(y - CCamera::GetInstance()->GetCamPos().y);
		r = static_cast<int>(l + CGame::GetInstance()->GetBackBufferWidth());
		b = static_cast<int>(t + CGame::GetInstance()->GetBackBufferHeight());

		x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2);
		y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2);
	}
	else 
	{
		l = static_cast<int>(x - CGame::GetInstance()->GetBackBufferWidth());
		t = static_cast<int>(y - CGame::GetInstance()->GetBackBufferHeight());
		r = static_cast<int>(l + CGame::GetInstance()->GetBackBufferWidth());
		b = static_cast<int>(t + CGame::GetInstance()->GetBackBufferHeight());
	}

	CGame::GetInstance()->Draw(x, y, bbox, l, t, r, b, Alpha);
}

void CBlackScreen::SetState(int state)
{
	switch (state)
	{
	case BLACK_SCR_EFF_STATE_DRAW_FROM_0:
		Alpha = 0.0f;
		Render();
		break;

	case BLACK_SCR_EFF_STATE_DRAW_FROM_1:
		Alpha = 1.0f;
		Render();

	case BLACK_SCR_EFF_STATE_STOP_DRAW:
		Alpha = 1.0f;

		break;
	}

	this->state = state;
}