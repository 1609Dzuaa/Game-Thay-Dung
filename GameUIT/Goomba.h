#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.035f
#define GOOMBA_READY_TO_FLY_SPEED 0.274f
#define GOOMBA_FLYING_SPEED 0.47f
#define GOOMBA_DIE_REVERSE_FACTOR_X 1.7f
#define GOOMBA_DIE_REVERSE_FACTOR_Y 0.4f

#define GOOMBA 1
#define PARA_GOOMBA 2
#define STICK_WITH_KOOPA 3

#define PARA_GOOMBA_LEVEL_NO_WINGS 1
#define PARA_GOOMBA_LEVEL_HAS_WINGS 2

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define PARA_GOOMBA_BBOX_WIDTH 19
#define PARA_GOOMBA_BBOX_HEIGHT 18
#define PARA_GOOMBA_SPREAD_WINGS_BBOX_WIDTH 19
#define PARA_GOOMBA_SPREAD_WINGS_BBOX_HEIGHT 21

#define GOOMBA_DIE_TIMEOUT 700
#define GOOMBA_CALM_TIMEOUT 500
#define GOOMBA_FLY_TIMEOUT 400

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_REVERSE 201
#define GOOMBA_STATE_READY_TO_FLY 202
#define GOOMBA_STATE_FLYING 203

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_DIE_REVERSE 5002

#define ID_ANI_PARA_GOOMBA_NO_WINGS_WALKING 5100
#define ID_ANI_PARA_GOOMBA_DIE 5101
#define ID_ANI_PARA_GOOMBA_DIE_REVERSE 5102
#define ID_ANI_PARA_GOOMBA_HAS_WINGS_WALKING 5103
#define ID_ANI_PARA_GOOMBA_READY_TO_FLY 5104
#define ID_ANI_PARA_GOOMBA_FLYING 5105

class CGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG die_reverse_start;
	ULONGLONG fly_start;
	BOOLEAN isDead;
	BOOLEAN isSpreadWings;
	int type, level, count_step, count_step_to_jump;
	int isFallOff;
	//count_step: Dùng để đếm bước để bắt đầu nhảy thấp
	//count_step_to_jump: Dùng để đếm bước để bắt đầu nhảy cao

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void UpdateGoombaState();
	void HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e);

    int IsCollidable() 
	{ 
		return (state != GOOMBA_STATE_DIE &&
			state != GOOMBA_STATE_DIE_REVERSE); 
		//Nó phải khác cả 2 trạng thái chết bình thường và chết NGƯỢC thì mới đúng
	};
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CGoomba(float x, float y, int type);
	virtual void SetState(int state);
	int GetType() { return this->type; };
	void SetLevel(int para) { this->level = para; this->state = GOOMBA_STATE_WALKING; }
	int GetLevel() { return this->level; }
	void OnCollisionWithGhostPlatform(LPCOLLISIONEVENT e, CColorPlatform* cl_pf);
	void SnappingToAnEdge();
};