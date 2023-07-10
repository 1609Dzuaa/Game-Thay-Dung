#include "RedArrow.h"

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