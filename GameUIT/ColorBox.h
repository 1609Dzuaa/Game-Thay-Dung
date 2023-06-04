#pragma once
#include "GameObject.h"

#define ID_ANI_COLOR_BOX 90000

class CColorBox :public CGameObject
{
	CColorBox(float x, float y) :CGameObject(x, y) {};
public:
	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	void Render();
};