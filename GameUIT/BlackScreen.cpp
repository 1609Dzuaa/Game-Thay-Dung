﻿#include "BlackScreen.h"
#include "Camera.h"
#include "Textures.h"
#include "PlayScene.h"
#include "debug.h"

CBlackScreen* CBlackScreen::__BlackScrInstance = nullptr;

CBlackScreen* CBlackScreen::GetInstance()
{
	if (__BlackScrInstance == nullptr)
	{
		float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
		float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() / 2;
		__BlackScrInstance = new CBlackScreen(x, y);
		DebugOut(L"Create Black Screen Successfully\n");
	}
	return __BlackScrInstance;
}

void CBlackScreen::Update()
{
	float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
	float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() / 2;
	this->x = x;
	this->y = y;

	if (state == BLACK_SCR_EFF_STATE_DRAW_FROM_0)
	{
		Alpha += 0.02f;
		if (Alpha >= 1.0f)
		{
			Alpha = 1.0f;
			state = BLACK_SCR_EFF_STATE_DRAW_FROM_1;
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->SetIsWaitingForTrans(0);
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

	//DebugOutTitle(L"St, Al: %d, %f", state, Alpha);
}

void CBlackScreen::Render()
{
	//Lấy ý tưởng từ hàm RenderBoundingBox trong Framework, vẽ cái screen mờ đi theo độ Alpha

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_BLACK_SCREEN);

	int l, t, r, b;
	//Done Warning here!
	l = static_cast<int>(x - CCamera::GetInstance()->GetCamPos().x);
	t = static_cast<int>(y - CCamera::GetInstance()->GetCamPos().y);
	r = static_cast<int>(l + CGame::GetInstance()->GetBackBufferWidth());
	b = static_cast<int>(t + CGame::GetInstance()->GetBackBufferHeight());

	x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2);
	y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2);

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