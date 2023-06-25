#pragma once
#include "GameObject.h"

#define ANI_SWITCH 20300
#define ANI_SWITCH_HITTED	20310

#define SWITCH_STATE_NORMAL	1
#define SWITCH_STATE_HITTED	0

#define SWITCH_BBOX_WIDTH 16
#define SWITCH_BBOX_HEIGHT 16

class CSwitch :public CGameObject 
{
public:
	CSwitch(float x, float y) :CGameObject(x, y) 
	{
		state = SWITCH_STATE_NORMAL;
	}
	void Update(DWORD dt);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetState(int state);
};