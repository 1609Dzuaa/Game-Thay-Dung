#pragma once
#include "GameObject.h"

class CGrass :public CGameObject
{
public:
	CGrass(float x, float y) :CGameObject(x, y) {};
	void Render() {};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	//void RenderBoundingBox() {};
};