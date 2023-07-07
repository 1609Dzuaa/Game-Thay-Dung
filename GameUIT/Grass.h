#pragma once
#include "GameObject.h"

#define ID_GRASS	3150

class CGrass :public CGameObject
{
public:
	CGrass(float x, float y) :CGameObject(x, y) {};
	void Render() 
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_GRASS)->Render(x, y, false);
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};