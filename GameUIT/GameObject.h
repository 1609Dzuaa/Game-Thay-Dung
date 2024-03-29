﻿#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:

	float x;//, x_initial;
	float y;// , y_initial;

	float vx;
	float vy;

	int nx;

	int state;

	int IsWaitable, IsWaiting;

	bool isDeleted;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	float GetX() { return x; }
	float GetY() { return y; }
	//Hàm GetSpeed lấy tốc độ của đối tượng và lưu nó vào tham số

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true; }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject()
	{
		this->x = x;
		this->y = y;
		this->IsWaitable = false;
		this->IsWaiting = false;
	}


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) 
	{
		if (IsWaiting && IsWaitable) //Nếu vật đang đợi VÀ CÓ THỂ ĐỢI 
			return;
	};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void SetWait(int wait) { this->IsWaiting = wait; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	//Khởi tạo ban đầu của mọi object trong game là CÓ thuộc tính block(return 1;) và KHÔNG THỂ va chạm(return 0;)
	//Còn sau đó obj đó có thuộc tính block hay không là tuỳ định nghĩa ở mỗi class

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; }
	virtual int IsSpecialItem() { return 0; } //Khởi tạo mặc định mọi vật thể trong game đều 0 PHẢI item ĐẶC BIỆT
};
