#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"
#include "Mario.h"
#include "MarioNPC.h"
#include "Koopa.h"

#define LUIGI_NPC_WALKING_SPEED	0.15f
#define LUIGI_NPC_JUMP_SPEED	0.5f

#define LUIGI_NPC_STATE_IDLE	0
#define LUIGI_NPC_STATE_WALKING	10
#define LUIGI_NPC_STATE_JUMPING	20

#define ID_ANI_LUIGI_IDLE_RIGHT	700000
#define ID_ANI_LUIGI_IDLE_LEFT	700050

#define ID_ANI_LUIGI_WALKING_RIGHT	700100	

#define ID_ANI_LUIGI_JUMPING_RIGHT	700200

#define ID_ANI_LUIGI_HOLD_WALK_LEFT	700301

#define ID_ANI_LUIGI_HOLD_LEFT	700300

#define ID_ANI_LUIGI_KICK_LEFT	700400

class CLuigiNPC : public CGameObject
{
	float ax, ay;
	int level;
	int isOnPlatform;
	BOOLEAN isKicking, isHolding;
	BOOLEAN isJumping, isRunning;
	BOOLEAN hasJumped, hasSpawn;
	BOOLEAN initKoopa, hasKick;
	float maxVx;
	ULONGLONG kick_start;
	CKoopa* ghost_koopa;
public:
	CLuigiNPC(float x, float y) :CGameObject(x, y)
	{
		this->nx = 1;
		ax = MARIO_ACCEL_WALK_X;
		ay = 0.002f;
		isOnPlatform = 0;
		isKicking = 0;
		isHolding = 0;
		maxVx = 0.0f;
		kick_start = 0;
		isJumping = 0;
		initKoopa = 0;
		hasKick = 0;
		SetState(MARIO_STATE_WALKING_RIGHT);

		hasJumped = 0;
		hasSpawn = 0;
		ghost_koopa = nullptr;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateSpeed();
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithMarioNPC(LPCOLLISIONEVENT e);
	void Render();
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	int GetAniID();

	void UpdateKoopaPos();
	void HandleReleaseKoopa();
	void SpawnSuperMarioBros3();
};
