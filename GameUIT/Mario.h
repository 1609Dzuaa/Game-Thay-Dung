#pragma once
#include "GameObject.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_KICKING_RIGHT	110
#define MARIO_STATE_KICKING_LEFT	210

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_RACOON_STATE_ATTACK 700

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_KICKING_LEFT 1010
#define ID_ANI_MARIO_KICKING_RIGHT 1011

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_KICKING_LEFT 1410
#define ID_ANI_MARIO_SMALL_KICKING_RIGHT 1411

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_SIT_RIGHT 1700
#define ID_ANI_MARIO_SMALL_SIT_LEFT 1701

//
//RACOON MARIO
//

#define ID_ANI_MARIO_RACOON_IDLE_LEFT 1800
#define ID_ANI_MARIO_RACOON_IDLE_RIGHT 1801

#define ID_ANI_MARIO_RACOON_WALKING_LEFT 1900
#define ID_ANI_MARIO_RACOON_WALKING_RIGHT 1901

#define ID_ANI_MARIO_RACOON_RUNNING_LEFT 2000
#define ID_ANI_MARIO_RACOON_RUNNING_RIGHT 2001

#define ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT 2100
#define ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT 2101

#define ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT 2200
#define ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT 2201

#define ID_ANI_MARIO_RACOON_BRACE_LEFT 2400
#define ID_ANI_MARIO_RACOON_BRACE_RIGHT 2401

#define ID_ANI_MARIO_RACOON_SIT_RIGHT 2500
#define ID_ANI_MARIO_RACOON_SIT_LEFT 2501

#define ID_ANI_MARIO_RACOON_KICKING_RIGHT 2600
#define ID_ANI_MARIO_RACOON_KICKING_LEFT 2601

#define ID_ANI_MARIO_RACOON_ATTACKING_RIGHT 2700
#define ID_ANI_MARIO_RACOON_ATTACKING_LEFT 2701

#pragma endregion

#define GROUND_Y 160.0f


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON 3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_RACOON_BBOX_WIDTH  24 //USED TO BE 21
#define MARIO_RACOON_BBOX_HEIGHT 24
#define MARIO_RACOON_SITTING_BBOX_WIDTH  22
#define MARIO_RACOON_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2)
#define MARIO_RACOON_SIT_HEIGHT_ADJSUT ((MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_SITTING_BBOX_HEIGHT) / 2)

#define MARIO_SMALL_BBOX_WIDTH  12 //USED TO BE 13
#define MARIO_SMALL_BBOX_HEIGHT 12 //CALCULATE RA 16 17 ??


#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_KICK_TIME 100
#define MARIO_RACOON_ATTACK_TIME 300

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isKicking;
	BOOLEAN isAttacking;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	ULONGLONG kick_start; //Tính thời gian sút để giải phóng Mario khỏi hành động sút
	ULONGLONG attack_start;
	BOOLEAN isOnPlatform;
	int coin;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithQuesBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

	void HandleCollisionUpperDirectionWithGoomba(CGoomba* goomba);
	void HandleCollisionOtherDirectionWithGoomba(LPCOLLISIONEVENT e, CGoomba* goomba); 
	void HandleCollisionUpperDirectionWithKoopa(CKoopa* koopa);
	void HandleCollisionOtherDirectionWithKoopa(LPCOLLISIONEVENT e, CKoopa* koopa);

	void KindOfOnCollisionWith(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isKicking = false;
		isAttacking = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		kick_start = 0;
		attack_start = 0;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }
	//Để ý ở đây Mario có thuộc tính blocking => đôi lúc nó sẽ khiến các quái vật khác đổi hướng khi va chạm với nó
	//Khi nó ở trạng thái vô địch(untouchable) và CHƯA CHẾT

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; };
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	float GetMarioPositionX() { return x; };
	int GetMarioNormalX() { return nx; }
};