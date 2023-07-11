#pragma once
#include "GameObject.h"

#define RED_CURTAIN_SPEED_UP	0.2f

#define ID_ANI_RED_CURTAIN	3160

class CRedCurtain : public CGameObject
{
public:
	CRedCurtain(float x, float y) :CGameObject(x, y) 
	{ 

	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	void Render();
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state);
};