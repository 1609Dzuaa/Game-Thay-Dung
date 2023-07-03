#include "Effect.h"
#include "Camera.h"
#include "Textures.h"
#include "PlayScene.h"
#include "debug.h"

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CEffect::Render()
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

void CEffect::SetState(int state)
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

	CGameObject::SetState(state);
}