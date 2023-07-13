#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"
#include "Mario.h"

//Ráng lên, Try-hard để kh phải hối tiếc ~~
//NPC Landing làm như nào ?: thử làm như draw, undraw, có bộ đếm giờ, draw thì vy=0 ?

#define MARIO_NPC_WALKING_SPEED	0.15f
#define MARIO_NPC_JUMP_SPEED	0.5f

#define MARIO_STATE_LOOKUP	30	//Đặt tạm đây để compile cho nhanh, sau nhớ chuyển lại về file Mario.h

#define ID_ANI_MARIO_HITTED_BY_SHELL	1081
#define ID_ANI_MARIO_LOOK_UP	1091

class CMarioNPC : public CGameObject
{
	float ax, ay;
	int level;
	int isOnPlatform;
	int allowLanding; //Chẵn lẻ xen kẽ (chẵn: Landing, lẻ: Fall)
	BOOLEAN isKicking, isHolding, isEvolving, isEvolveForward, isEvolveBackward;
	BOOLEAN isJumping, isLanding, isRunning, isSitting, isHitted, isLookUp;
	BOOLEAN hasKilledGoomba, hasKickKoopa1, hasKickKoopa2, hasSpawnKoopa;
	BOOLEAN hasTurnSmall, hasMoveRight, hasBrace, hasSpawnBush, hasSpawn4Koopas;
	float maxVx;
	ULONGLONG kick_start, evolve_start, hitted_timeout, look_timeout, idle_start;
	
	//Đặt đây 0 hợp lý lắm, mai xem xét lại
	CKoopa* koopa1 = nullptr;
	CKoopa* koopa2 = nullptr;
	CKoopa* koopa3 = nullptr;
	CKoopa* koopa4 = nullptr;
public:
	CMarioNPC(float x, float y) :CGameObject(x, y)
	{
		this->nx = -1;
		ax = MARIO_ACCEL_WALK_X;
		vx = -0.01f;
		ay = 0.002f;
		level = MARIO_LEVEL_BIG;
		isOnPlatform = 0;
		isKicking = 0;
		isHolding = 0;
		isEvolving = 0;
		isEvolveForward = 0;
		isEvolveBackward = 0;
		maxVx = 0.0f;
		kick_start = 0;
		evolve_start = 0;
		isJumping = 0;
		isLanding = 0;
		isRunning = 0;
		isSitting = 0;
		isHitted = 0;
		hitted_timeout = 0;
		isLookUp = 0;
		look_timeout = 0;
		hasKilledGoomba = 0;
		hasKickKoopa1 = 0;
		hasKickKoopa2 = 0;
		allowLanding = 0;
		hasSpawnKoopa = 0;
		hasTurnSmall = 0;
		hasMoveRight = 0;
		hasBrace = 0;
		idle_start = 0;
		hasSpawnBush = 0;
		hasSpawn4Koopas = 0;
		SetState(MARIO_STATE_WALKING_LEFT);
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateSpeed();
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	void Render();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithBlockingObject(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void HandleEvolving();
	void SetLevel(int level);
	void SpawnEffect(LPCOLLISIONEVENT e, LPGAMEOBJECT obj, int eff_type); //nên cho vào class EffectCol
	void SpawnSlipingKoopaFromLeft();
	void SpawnBigBush();
	void Spawn4KoopasFromLeft();

	//Quên mất thằng này, 0 có nó thì 0 colli đc @@@@@@@@@
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();
};