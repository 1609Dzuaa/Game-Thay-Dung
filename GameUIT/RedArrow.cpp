#include "RedArrow.h"
#include "Scene.h"
#include "AssetIDs.h"

CRedArrow* CRedArrow::__RedArrowInstance = nullptr;

CRedArrow* CRedArrow::GetInstance()
{
	if (!__RedArrowInstance)
	{
		float x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2) - 15.0f;
		float y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) - 65.0f;
		__RedArrowInstance = new CRedArrow(x, y);
	}
	return __RedArrowInstance;
}

void CRedArrow::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_RED_ARROW)->Render(x, y, false);
}

void CRedArrow::SetLevel(int para)
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
	if (current_scene->GetID() == ID_WORLD)
	{
		if (!para)
		{
			this->x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2) - 15.0f;
			this->y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) - 65.0f;
		}
		else
		{
			this->x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2) - 15.0f;
			this->y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) - 56.0f;
		}
	}
	else //At Intro
	{
		if (!para)
		{
			this->x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2) - 52.0f;
			this->y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) + 6.0f;
		}
		else
		{
			this->x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2) - 52.0f;
			this->y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) + 22.0f;
		}
	}
}