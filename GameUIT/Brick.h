#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16

#define ID_ANI_BULLET 20000

class CBrick : public CGameObject 
{
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};

class CFireBullet : public CGameObject
{
public:
	CFireBullet() : CGameObject(x, y)
	{
		this->vx = 0.05f;
	}
	void Render();
	void Update(DWORD dt);
	void SetState(int state);
	void Fire(int para);
};