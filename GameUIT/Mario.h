#pragma once
#include "GameObject.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Animation.h"
#include "Animations.h"
#include "ColorPlatform.h"
#include "Effect.h"
#include "EffectScore.h"
#include "Tail.h"

#include "debug.h"

//Task:
//Finish Flying & Landing: 95%
//Adjust Constant
//Finish Tail: not yet
//Initialize Use Tube Switch Scene: nope
//Render Underworld may has bug cuz of Render func in GameMap: will check it later

#pragma region CONST & TIME

#define MARIO_WALKING_SPEED		0.11f
#define MARIO_RUNNING_SPEED		0.15f

#define MARIO_ACCEL_WALK_X	0.0001f
#define MARIO_ACCEL_RUN_X	0.00012f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.5f
//#define MARIO_FLYING_SPEED	0.315f;
#define MARIO_FLYING_SPEED	0.4f;
#define MARIO_TRAVELLING_SPEED	0.02815f

//#define MARIO_GRAVITY	0.001f
#define MARIO_GRAVITY	0.0018f

#define MARIO_JUMP_DEFLECT_SPEED  0.004f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16 //Big & Racoon share the same Height when SIT

#define MARIO_EVOLVE_BBOX_WIDTH 14
#define MARIO_EVOLVE_BBOX_HEIGHT 20
#define MARIO_RACOON_BBOX_WIDTH  24 

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2)

#define MARIO_SMALL_BBOX_WIDTH  13 
#define MARIO_SMALL_BBOX_HEIGHT 12 

#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_KICK_TIME 150
#define MARIO_RACOON_ATTACK_TIME 370
#define MARIO_EVOLVE_TIME 750
#define UNTOUCH_DRAW_TIME 100
#define FLYING_TIME	4000

#pragma endregion CONST & TIME

//==================================================//
#pragma region STATE

#define MARIO_STATE_DIE				-10

#define MARIO_STATE_IDLE			0

#define MARIO_STATE_WALKING_RIGHT	100

#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_KICKING_RIGHT	110

#define MARIO_STATE_KICKING_LEFT	210

#define MARIO_STATE_JUMPING			300

#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400

#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_RUNNING_AT_MAX_SPEED_RIGHT 401

#define MARIO_STATE_RUNNING_AT_MAX_SPEED_LEFT 402

#define MARIO_STATE_SIT				600

#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_RACOON_STATE_ATTACK 700

#define MARIO_RACOON_STATE_LANDING 710 //gộp Falling chung với Landing (Falling khi Vy > 0, Landing khi Vy = 0)

#define MARIO_RACOON_STATE_FLYING 720

#define MARIO_STATE_EVOLVING 725

#define MARIO_STATE_JUMP_AT_MAX_SPEED 730

#define MARIO_STATE_HOLDING 735

#define MARIO_STATE_TRAVELLING	740

#pragma endregion STATE
//==================================================//

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

#define ID_ANI_MARIO_BIG_HOLDING_LEFT 1020
#define ID_ANI_MARIO_BIG_HOLDING_RIGHT 1021

#define ID_ANI_MARIO_BIG_RUNNING_MAX_SPEED_LEFT	1030	
#define ID_ANI_MARIO_BIG_RUNNING_MAX_SPEED_RIGHT	1031

#define ID_ANI_MARIO_BIG_HOLDING_WAKING_LEFT	1040	
#define ID_ANI_MARIO_BIG_HOLDING_WAKING_RIGHT	1041

#define ID_ANI_MARIO_BIG_JUMP_AT_MAX_SPEED_LEFT	1050	
#define ID_ANI_MARIO_BIG_JUMP_AT_MAX_SPEED_RIGHT	1051

#define ID_ANI_MARIO_BIG_HOLDING_JUMPING_LEFT	1060	
#define ID_ANI_MARIO_BIG_HOLDING_JUMPING_RIGHT	1061

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1101

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

#define ID_ANI_MARIO_SMALL_RUNNING_MAX_SPEED_RIGHT 1750
#define ID_ANI_MARIO_SMALL_RUNNING_MAX_SPEED_LEFT 1751

#define ID_ANI_MARIO_SMALL_JUMP_AT_MAX_SPEED_LEFT	1760	
#define ID_ANI_MARIO_SMALL_JUMP_AT_MAX_SPEED_RIGHT	1761

#define ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_LEFT 1770
#define ID_ANI_MARIO_SMALL_EVOLVE_TO_BIG_RIGHT 1771

#define ID_ANI_MARIO_SMALL_HOLD_LEFT 1775
#define ID_ANI_MARIO_SMALL_HOLD_RIGHT 1776

#define ID_ANI_MARIO_SMALL_HOLD_WALK_LEFT 1780
#define ID_ANI_MARIO_SMALL_HOLD_WALK_RIGHT 1781

#define ID_ANI_MARIO_SMALL_HOLD_JUMP_LEFT 1785
#define ID_ANI_MARIO_SMALL_HOLD_JUMP_RIGHT 1786

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

#define ID_ANI_MARIO_RACOON_FALLING_RIGHT 2800
#define ID_ANI_MARIO_RACOON_FALLING_LEFT 2801

#define ID_ANI_MARIO_RACOON_RUNNING_MAX_SPEED_LEFT	2810	
#define ID_ANI_MARIO_RACOON_RUNNING_MAX_SPEED_RIGHT	2811

#define ID_ANI_MARIO_RACOON_JUMP_AT_MAX_SPEED_LEFT	2820	
#define ID_ANI_MARIO_RACOON_JUMP_AT_MAX_SPEED_RIGHT	2821

#define ID_ANI_MARIO_RACOON_LANDING_LEFT	2830	
#define ID_ANI_MARIO_RACOON_LANDING_RIGHT	2831

#define ID_ANI_MARIO_RACOON_HOLD_LEFT 2835
#define ID_ANI_MARIO_RACOON_HOLD_RIGHT 2836

#define ID_ANI_MARIO_RACOON_HOLD_WALK_LEFT 2840
#define ID_ANI_MARIO_RACOON_HOLD_WALK_RIGHT 2841

#define ID_ANI_MARIO_RACOON_HOLD_JUMP_LEFT 2845
#define ID_ANI_MARIO_RACOON_HOLD_JUMP_RIGHT 2846

#define ID_ANI_MARIO_RACOON_FLYING_LEFT	2850	
#define ID_ANI_MARIO_RACOON_FLYING_RIGHT	2851

#define ID_ANI_MARIO_RACOON_TRAVELLING	2860

#pragma endregion

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isWalking;
	BOOLEAN isRunning;
	BOOLEAN isKicking;
	BOOLEAN isAttacking;
	BOOLEAN isJumping;
	BOOLEAN canFly;
	BOOLEAN isAtMaxSpeed;
	BOOLEAN isLanding;
	BOOLEAN isEvolving;
	BOOLEAN isEvolveForward;
	BOOLEAN isEvolveBackward;
	BOOLEAN isAteItem; //Biến đặc biệt dùng để nhận biết xem được tăng level nhờ item hay nhấn phím, mục đích xem ở hàm SetLevel
	BOOLEAN StopWatch; //ngưng mọi hoạt động khi Mario đang tiến hoá hoặc chết
	BOOLEAN Shaking;
	BOOLEAN isFlying;
	BOOLEAN isAllowToHoldKoopa; //Allow to hold rồi mới holding
	BOOLEAN isHolding; //A way to handle holding: 
	BOOLEAN isHitSwitch;
	BOOLEAN isInitialized;
	BOOLEAN isAllowToUseTube;
	BOOLEAN isTravelling;
	BOOLEAN isAtMainWorld;
	CKoopa* ghost_koopa; //khi đang giữ Koopa, coi nó như item của mình, đang bật khiên, đụng là chết
	//về cơ bản cũng khá giống cái đuôi, cũng cần đc vẽ bbox
	float maxVx;
	float maxRunningSpeed;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float start_y;			//Điểm bắt đầu chui xuống
	CTail* tail;
	CEffect* black_eff;

	int level;
	int untouchable, untouchdraw;
	ULONGLONG untouchable_start;
	ULONGLONG kick_start; //Tính thời gian sút để giải phóng Mario khỏi hành động sút
	ULONGLONG attack_start;
	ULONGLONG evolve_start;
	ULONGLONG fly_start;
	BOOLEAN untouch_0; // cờ 0 vẽ
	BOOLEAN untouch_1; // cờ vẽ
	ULONGLONG untouch_draw_0; //thgian 0 vẽ
	ULONGLONG untouch_draw_1; //thgian vẽ
	ULONGLONG shaking_start;
	ULONGLONG shakeUp_start, shakeDown_start;
	BOOLEAN isShakeUp;
	BOOLEAN isOnPlatform;
	BOOLEAN isTravelUp;
	BOOLEAN isTravelDown;
	BOOLEAN isComboUpAndS;
	BOOLEAN isReachTransPos; //Dùng để nhận biết đã chạm tới điểm trans
	BOOLEAN isWaitingForTrans;
	int CountJumpOnEnemies; //Đếm số bước nhảy 0 CHẠM ĐẤT để có số điểm tương ứng

	//Collision Func
	void OnCollisionWithColorPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithQuesBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithFlower(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitch(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBlockingObjects(LPCOLLISIONEVENT e);
	void OnCollisionWithNonBlockingObjects(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	//Handle Func
	//Tách nhỏ handleOtherDirecKoopa thành lúc ngủ và lúc không ngủ
	void HandleCollisionUpperDirectionWithGoomba(CGoomba* goomba);
	void HandleCollisionOtherDirectionWithGoomba(LPCOLLISIONEVENT e, CGoomba* goomba);
	void HandleCollisionUpperDirectionWithKoopa(CKoopa* koopa);
	void HandleCollisionOtherDirectionWithKoopa(LPCOLLISIONEVENT e, CKoopa* koopa);
	void HandleCollisionWithColorPlatform(LPCOLLISIONEVENT e, CColorPlatform* color_platf);
	void HandleUntouchableDrawState(); //hiệu ứng chớp chớp 

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();
	int KoopaStateThatAllowToHold(CKoopa* koopa);
public:
	CMario(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateMarioState();
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE && !isTravelling);
	}

	int IsBlocking() { return 0; }
	//Để ý ở đây Mario có thuộc tính blocking => đôi lúc nó sẽ khiến các quái vật khác đổi hướng khi va chạm với nó
	//Khi nó ở trạng thái vô địch(untouchable) và CHƯA CHẾT

	void SetLevel(int l);
	int GetLevel() { return level; };
	void StartUntouchable() 
	{ untouchable = 1; untouchable_start = GetTickCount64(); untouch_0 = 1; untouch_draw_0 = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int GetMarioNormalX() { return nx; }
	BOOLEAN GetCanFly() { return canFly; }
	BOOLEAN GetisAtMaxSpeed() { return isAtMaxSpeed; }
	void SetisJumping(BOOLEAN para) { this->isJumping = para; }
	void SetisAtMaxSpeed(BOOLEAN para) { this->isAtMaxSpeed = para; }
	BOOLEAN GetIsJumping() { return isJumping; }
	CEffectScore* ClassifyScore(LPGAMEOBJECT obj, CEffectScore* eff_scr);
	void SetIsLanding(bool para) { this->isLanding = para; }
	void SpawnScore(LPGAMEOBJECT obj);
	void SpawnEffect(LPCOLLISIONEVENT e, LPGAMEOBJECT obj, int eff_type); //nên cho vào class EffectCol
	BOOLEAN GetStopWatch() { return evolve_start != 0 || this->state == MARIO_STATE_DIE; }
	BOOLEAN GetShaking() { return this->Shaking; }
	BOOLEAN GetIsFlying() { return this->isFlying; }
	BOOLEAN GetIsAttacking() { return this->isAttacking; }
	BOOLEAN GetIsHolding() { return this->isHolding; }
	BOOLEAN GetIsHitSwitch() { return this->isHitSwitch; }
	BOOLEAN GetIsSitting() { return this->isSitting; }
	BOOLEAN GetIsOnPlatform() { return this->isOnPlatform; }
	BOOLEAN GetIsAllowToUseTube() { return this->isAllowToUseTube; }
	BOOLEAN GetIsAtMainWorld() { return this->isAtMainWorld; }
	BOOLEAN GetIsTravelling() { return this->isTravelling; }
	void SetHoldKoopa(BOOLEAN para) { this->isAllowToHoldKoopa = para; }
	void SetIsHoldingKoopa(BOOLEAN para) { this->isHolding = para; }
	void SetIsHitSwitch(BOOLEAN para) { this->isHitSwitch = para; }
	void SetIsRunning(BOOLEAN para) { this->isRunning = para; }
	void SetIsAtMainWorld(BOOLEAN para) { this->isAtMainWorld = para; }
	void SetIsTravelUp(BOOLEAN para) { this->isTravelUp = para; }
	void SetIsTravelDown(BOOLEAN para) { this->isTravelDown = para; }
	void SetAllowToUseTube(BOOLEAN para) { this->isAllowToUseTube = para; }
	void SetCombo(BOOLEAN para) { this->isComboUpAndS = para; }
	void SetIsWaitingForTrans(BOOLEAN para) { this->isWaitingForTrans = para; }
	void HandleTravellingDown();
	void HandleTravellingUp();
};