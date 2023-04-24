#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

using namespace std;

class CGameObject
{
protected:
	float x;
	float y;

	float vx;
	float vy;

	int nx;

	int state;
public:
	float GetX() { return this->x; }
	float GetY() { return this->y; }

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetVX(float vx) { this->vx = vx; }

	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	~CGameObject() {};
};

typedef CGameObject* LPGAMEOBJECT;
