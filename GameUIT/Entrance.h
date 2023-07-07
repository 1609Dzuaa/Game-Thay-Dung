#pragma once
#include "GameObject.h"

class CEntrance :public CGameObject 
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsBlocking() { return 0; }
public:
	CEntrance();
};