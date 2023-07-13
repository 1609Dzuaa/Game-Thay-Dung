#pragma once
#include "GameObject.h"

#define ID_ANI_BIG_BUSH	500008

class CBigBush :public CGameObject
{
public:
	CBigBush(float x, float y) :CGameObject(x, y) {};
	void Render()
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_BIG_BUSH)->Render(x, y, true);
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};