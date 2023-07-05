#pragma once
#include "GameObject.h"

class CEffect : public CGameObject 
{
public:
	CEffect(float x, float y) { this->x = x; this->y = y; };
	~CEffect() {};
	int IsBlocking() { return 0; };
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void Render() {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};