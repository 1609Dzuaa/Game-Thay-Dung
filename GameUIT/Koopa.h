﻿#pragma once
#include "GameObject.h"
#include "ColorPlatform.h"
#include "Head.h"
#include "Goomba.h"

#pragma region KOOPA_TYPE 

//In map 1-1 there are 3 types of Koopa so I'll define each type here:
#define GREEN_KOOPA 1
#define GREEN_FLYING_KOOPA 2
#define RED_KOOPA 3
//Red Koopa khi đặt trên các vật thể: ống, quesbrick, gạch vàng, colorplat lẫn đi bộ thì đều đổi dấu
//Đi bộ 0 rơi xuống vực
//Red Koopa lâu lâu bị rớt xuống color plat, vụt Green Koopa vẫn còn vx trên color plat
//Vẫn còn bug red koopa
//Do vị trí đứng của Mario: ở bên trái Koopa thì bthg, ở bên phải thì lệch

#pragma endregion 

#define KOOPA_GRAVITY 0.001f
#define KOOPA_STEP_ON_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED 0.018f
#define KOOPA_SLIPPING_SPEED 0.15f
#define KOOPA_JUMP_SPEED 0.31f
#define KOOPA_KNOCK_OFF_VELO_X 0.1f
#define KOOPA_KNOCK_OFF_FACTOR_X 1.15f
#define KOOPA_KNOCK_OFF_FACTOR_Y 0.3f

#define KOOPA_BBOX_WIDTH 18
#define KOOPA_BBOX_HEIGHT 23
#define KOOPA_IN_SHELL_BBOX_HEIGHT 15

#define KOOPA_DIE_TIMEOUT 500
#define KOOPA_SLEEP_TIMEOUT 4500
#define KOOPA_REBORN_TIMEOUT 2500
#define KOOPA_KNOCK_OFF_TIMEOUT 500

//vụt nó lúc ngủ ngược không thấy vx đâu
#define KOOPA_STATE_WALKING 1000
#define KOOPA_STATE_SLEEP 2000
#define KOOPA_STATE_SLEEP_REVERSE 2050 //Trạng thái NGỦ LẬT MAI có vx để lúc vụt nó bay đi một đoạn dx
#define KOOPA_STATE_SLEEP_REVERSE_SPECIAL 2077 //Trạng thái NGỦ LẬT MAI mà không có vx (vx=0) tránh nó bị trượt lúc ngủ
#define KOOPA_STATE_SLIP 2100
#define KOOPA_STATE_SLIP_REVERSE 2150
#define KOOPA_STATE_DIE 2200
#define KOOPA_STATE_REBORN 2250
#define KOOPA_STATE_REBORN_REVERSE 2300 //Dùng cho lúc Koopa bị Gấu Mèo quật
#define KOOPA_STATE_JUMPING 2350
#define KOOPA_STATE_BEING_HELD 2400
#define KOOPA_STATE_BEING_HELD_REVERSE 2450
#define KOOPA_STATE_BOUNCING	2500	//For Black Koopa at IntroPlayScene

#define ID_ANI_KOOPA_WALKING_LEFT 50000
#define ID_ANI_KOOPA_WALKING_RIGHT 50500
#define ID_ANI_KOOPA_SLEEPING 50005
#define ID_ANI_KOOPA_SLEEPING_REVERSE 500010
#define ID_ANI_KOOPA_SLIPPING 50015
#define ID_ANI_KOOPA_SLIPPING_REVERSE 50020
#define ID_ANI_KOOPA_DIE 50025 
#define ID_ANI_KOOPA_REBORN 50030
#define ID_ANI_KOOPA_REBORN_REVERSE 50035
#define ID_ANI_KOOPA_JUMPING_LEFT 60000
#define ID_ANI_KOOPA_JUMPING_RIGHT 60500
#define ID_ANI_BLACK_KOOPA_SHELL 39100
#define ID_ANI_BLACK_KOOPA_DIE	39200

//
//======RED KOOPA
//

#define ID_ANI_RED_KOOPA_WALKING_LEFT 52000
#define ID_ANI_RED_KOOPA_WALKING_RIGHT 52500
#define ID_ANI_RED_KOOPA_SLEEPING 52005
#define ID_ANI_RED_KOOPA_SLEEPING_REVERSE 52010
#define ID_ANI_RED_KOOPA_SLIPPING 52015
#define ID_ANI_RED_KOOPA_SLIPPING_REVERSE 52020
#define ID_ANI_RED_KOOPA_DIE 52025 
#define ID_ANI_RED_KOOPA_REBORN 52030
#define ID_ANI_RED_KOOPA_REBORN_REVERSE 52035

class CKoopa : public CGameObject
{
protected:
	int type;
	int isGreen, isBlack;	//2 thuộc tính dành cho Koopa Intro
	int countBounce; //black koopa
	int nxNPC;

	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG sleep_start;
	ULONGLONG reborn_start;
	ULONGLONG knock_off_start;
	BOOLEAN isOnPlatform;
	BOOLEAN enableToChangeVx; //use for Red Koopa only
	CHead* ghost_head; //use for Red Koopa
	BOOLEAN isBeingHeld;
	BOOLEAN isInitialized;
	//Red Koopa always has ghost_head

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() 
	{ 
		return state != KOOPA_STATE_DIE;
	};

	int IsBlocking() { return 0; };
	int KillCount; //Đếm số địch đã giết để hiện điểm tương ứng

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void KindOfCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithQuesBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithFlower(LPCOLLISIONEVENT e);
	void UpdateKoopaState();
	void HandleCollisionWithBlockingObjects(LPCOLLISIONEVENT e);
	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);
	void AdjustPositionOnColorPlatform(CColorPlatform* color_platf);

	int GetAniIdGreenKoopa();
	int GetAniIdFlyingKoopa();
	int GetAniIdRedKoopa();
public:
	CKoopa(float x, float y, int type);
	CKoopa(float x, float y, int isGreen, int isBlack); //Use for Intro
	virtual void SetState(int state);
	int GetType() { return type; };
	void SetType(int para) { this->type = para; };
	CHead* GetHead() { return this->ghost_head; }
	void SetBeingHeld(BOOLEAN para) { this->isBeingHeld = para; }
	int StateThatEnableToRelease();
	int ConditionsThatEnableToKillAllies();
	void SetNxNPC(int nx) { this->nxNPC = nx; }
	void SetVx(float vx) { this->vx = vx; }
};