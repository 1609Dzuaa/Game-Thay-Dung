#pragma once
#include "GameObject.h"

#pragma region KOOPA_TYPE 

//In map 1-1 there are 3 types of Koopa so I'll define each type here:
#define GREEN_KOOPA 1
#define GREEN_FLYING_KOOPA 2
#define RED_KOOPA 3

#pragma endregion 

#define KOOPA_GRAVITY 0.002f
#define KOOPA_STEP_ON_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_JUMP_SPEED 0.35f


#define KOOPA_BBOX_WIDTH 17
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_IN_SHELL_HEIGHT 15

#define KOOPA_DIE_TIMEOUT 500

#define KOOPA_STATE_WALKING 1000
#define KOOPA_STATE_SLEEP 2000
#define KOOPA_STATE_SLIP 2050
#define KOOPA_STATE_DIE 2100
#define KOOPA_STATE_REBORN 2200
#define KOOPA_STATE_JUMPING 12300

#define ID_ANI_KOOPA_WALKING_LEFT 50000
#define ID_ANI_KOOPA_WALKING_RIGHT 50500
#define ID_ANI_KOOPA_SLEEPING 50005
#define ID_ANI_KOOPA_SLIPPING 50010
#define ID_ANI_KOOPA_DIE 50015  //NULL HERE
#define ID_ANI_KOOPA_REBORN 50020
#define ID_ANI_KOOPA_JUMPING_LEFT 60000
#define ID_ANI_KOOPA_JUMPING_RIGHT 60500

class CKoopa : public CGameObject
{
protected:
	int type;

	float ax;
	float ay;

	ULONGLONG die_start;
	BOOLEAN isOnPlatform;
	BOOLEAN isStepOn;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() 
	{ 
		return state != KOOPA_STATE_DIE;
	};

	virtual int IsBlocking() { return 0; };

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithQuesBrick(LPCOLLISIONEVENT e);

	int GetAniIdGreenKoopa();
	int GetAniIdFlyingKoopa();
public:
	CKoopa(float x, float y, int type);
	virtual void SetState(int state);
	int GetType() { return type; };
	void SetType(int para) { this->type = para; };
};