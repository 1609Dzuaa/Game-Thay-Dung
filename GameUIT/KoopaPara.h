#pragma once
#include "GameObject.h"
#include "Koopa.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_STEP_ON_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_PARA_JUMP_SPEED 0.35f


#define KOOPA_BBOX_WIDTH 17
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_IN_SHELL_HEIGHT 15


#define KOOPA_DIE_TIMEOUT 500
#define KOOPA_PARA_JUMP_TIME 200


#define KOOPA_PARA_STATE_WALKING 11000
#define KOOPA_PARA_STATE_SLEEP 12000
#define KOOPA_PARA_STATE_SLIP 12050
#define KOOPA_PARA_STATE_DIE 12100
#define KOOPA_PARA_STATE_REBORN 12200
#define KOOPA_PARA_STATE_JUMPING 12300
#define KOOPA_PARA_STATE_RELEASE_JUMPING 12301


#define ID_ANI_KOOPA_PARA_WALKING_LEFT 60000
#define ID_ANI_KOOPA_PARA_WALKING_RIGHT 60500
#define ID_ANI_KOOPA_PARA_SLEEPING 60005
#define ID_ANI_KOOPA_PARA_SLIPPING 60010
#define ID_ANI_KOOPA_PARA_DIE 60015  //NULL HERE
#define ID_ANI_KOOPA_PARA_REBORN 60020

class CKoopaPara : public CKoopa
{
protected:
	float ax;
	float ay;

	BOOLEAN isOnPlatform;
	BOOLEAN isStepOn;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable()
	{
		return state != KOOPA_PARA_STATE_DIE;
	};

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	//void OnCollisionWithQuesBrick(LPCOLLISIONEVENT e);

public:
	CKoopaPara(float x, float y);
	virtual void SetState(int state);

};