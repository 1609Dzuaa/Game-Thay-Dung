#pragma once
#include "GameObject.h"

class CGhostObject :public CGameObject 
{
public:
	CGhostObject(float x, float y) :CGameObject(x, y) {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	//void RenderBoundingBox();
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
};