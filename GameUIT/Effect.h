#pragma once
#include "GameObject.h"

class CEffect : public CGameObject 
{
public:
	CEffect(float x, float y) :CGameObject(x, y) {};
	~CEffect() {};
	int IsBlocking() { return 0; };
	void Update(DWORD dt) {};
	void Render() {};
};