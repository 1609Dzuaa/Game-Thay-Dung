#pragma once
#include "GameObject.h"

#define ID_ANI_NB3_RACOON	500004

class CNb3Racoon :public CGameObject
{
public:
	CNb3Racoon(float x, float y) :CGameObject(x, y) {};
	void Render()
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_NB3_RACOON)->Render(x, y, false);
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};