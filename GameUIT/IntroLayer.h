#pragma once
#include "GameObject.h"

#define ID_ANI_INTRO_LAYER	500000	//Lớp nền đen và nền Juventus

class CIntroLayer :public CGameObject
{
public:
	CIntroLayer(float x, float y) :CGameObject(x, y) {};
	void Render()
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_INTRO_LAYER)->Render(x, y, true);
	};
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};