#pragma once
#include "GameObject.h"
class CShootingFlower :public CGameObject 
{
public:
	CShootingFlower();
	void Update(DWORD dt);
	void Render();
	void Shoot();
};