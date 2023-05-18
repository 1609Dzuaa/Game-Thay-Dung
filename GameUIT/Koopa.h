#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f


#define KOOPA_BBOX_WIDTH 20
#define KOOPA_BBOX_HEIGHT 18
#define KOOPA_BBOX_HEIGHT_DIE 10

#define KOOPA_DIE_TIMEOUT 500

#define KOOPA_STATE_WALKING 1000
#define KOOPA_STATE_SLEEP 2000
#define KOOPA_STATE_DIE 2100
#define KOOPA_STATE_REBORN 2200


#define ID_ANI_KOOPA_WALKING 50000
#define ID_ANI_KOOPA_SLEEPING 50005
#define ID_ANI_KOOPA_DIE 50010
#define ID_ANI_KOOPA_REBORN 50015

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
};