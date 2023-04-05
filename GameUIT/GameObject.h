#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"

//Class này sẽ là Class chứa tất cả các loại nhân vật trong Game
//File header (.h) là file chứa toàn bộ khai báo của các hàm, lớp và biến, cùng với các hằng số

class CGameObject
{
protected:
	float x;
	float y;
	float radius;
	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }

	float GetX() { return x; }

	float GetY() { return y; }

	float GetRadius() //Lấy bán kính để xét va chạm(Collision)
	{
		return radius;
	}

	CGameObject(float x = 0.0f, float y = 0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	~CGameObject();
};
typedef CGameObject* LPGAMEOBJECT;

class CBrick : public CGameObject
{
	float vx, vy;
public:
	CBrick(float x, float y, float vx, float vy, LPTEXTURE texture) : CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
		this->radius = (float)(25 * sqrt(2)) / (float)2;
	}
	void Update(DWORD dt);
};

class CBall : public CGameObject
{
	float vx, vy;
	bool destroyed = false; //biến để check xem vật thể bị phá huỷ hay chưa
public:
	CBall(float x, float y, float vx, float vy, LPTEXTURE texture) : CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
		this->texture = texture;
		this->radius = (float)(25 * sqrt(2)) / (float)2;
	}

	bool getDes()
	{
		return this->destroyed;
	}

	void SetDes(bool para)
	{
		this->destroyed = para;
	}

	void Update(DWORD dt);

	~CBall();
};

class CMario : public CGameObject
{
	float vx;
	float vy;
	bool destroyed = false;
public:
	CMario(float x, float y, float vx, float vy, LPTEXTURE texture) :CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
		this->radius = (float)(25 * sqrt(2)) / (float)2;
		//Vì các sprite có dạng hình vuông với cạnh 25
		//nên bán kính sẽ tính như trên
	};
	void Update(DWORD dt);

	bool getDes() { return this->destroyed; }

	void setDes(bool param) { this->destroyed = param; }
};

class CTurtle :public CGameObject
{
	float vx, vy;
public:
	CTurtle(float x, float y, float vx, float vy, LPTEXTURE tex) :CGameObject(x, y, tex)
	{
		this->vx = vx;
		this->vy = vy;
		this->radius = (float)(25 * sqrt(2)) / (float)2; 
	}
	void Update(DWORD dt);
};