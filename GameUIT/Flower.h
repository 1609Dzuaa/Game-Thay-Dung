#pragma once
#include "GameObject.h"
#include "FireBullet.h"
#include "PlayScene.h"
#include "EffectCollision.h"

//Quite similiar to Mushroom
//Chui từ vật thể ra

//TYPE: 1. SHOOTIN RED
//2. CARNIVOUR
//3. SHOOTIN GREEN

#define FLOWER_TYPE_SHOOTING_RED 1
#define FLOWER_TYPE_SHOOTING_GREEN 2
#define FLOWER_TYPE_CARNIVOURING_GREEN 3

#define ATTACK_RANGE 90.0f
#define FLOWER_RISE_SPEED 0.03f
#define FLOWER_GRAVITY	0.00001f

#define FLOWER_REST_TIME 1500
#define FLOWER_AIM_TIME 500

#define FLOWER_WIDTH 16
#define FLOWER_HEIGHT 32
#define GREEN_FLOWER_HEIGHT 24

#define FLOWER_STATE_IN_TUBE -1 //Chờ được vẽ và cập nhật
#define FLOWER_STATE_DIVE 0 //dive tới một đoạn nhất định trong ống
#define FLOWER_STATE_RISE_UP 1 //Đc Vẽ và cập nhật
#define FLOWER_STATE_ATTACK 2
#define FLOWER_STATE_REST 3
#define FLOWER_STATE_DIE 4

//Mặc định không có face up thì là face down
#define ID_ANI_FLOWER_RISE_UP_LEFT 81500
#define ID_ANI_FLOWER_RISE_UP_RIGHT 81600
#define ID_ANI_FLOWER_RISE_UP_LEFT_FACE_UP 81530    
#define ID_ANI_FLOWER_RISE_UP_RIGHT_FACE_UP 81630
#define ID_ANI_FLOWER_SHOOT_LEFT 81550
#define ID_ANI_FLOWER_SHOOT_RIGHT 81650
#define ID_ANI_FLOWER_SHOOT_LEFT_FACE_UP 81560
#define ID_ANI_FLOWER_SHOOT_RIGHT_FACE_UP 81660
#define ID_ANI_FLOWER_DIVE_LEFT	81700
#define ID_ANI_FLOWER_DIVE_RIGHT 81750
#define ID_ANI_FLOWER_DIVE_LEFT_FACE_UP	81710
#define ID_ANI_FLOWER_DIVE_RIGHT_FACE_UP 81760

#define ID_ANI_FLOWER_CARNIVOUR_RISE 81810

#define ID_ANI_GREEN_FLOWER_RISE_UP_LEFT 82500
#define ID_ANI_GREEN_FLOWER_RISE_UP_RIGHT 82600
#define ID_ANI_GREEN_FLOWER_RISE_UP_LEFT_FACE_UP 82530    
#define ID_ANI_GREEN_FLOWER_RISE_UP_RIGHT_FACE_UP 82630
#define ID_ANI_GREEN_FLOWER_SHOOT_LEFT 82550
#define ID_ANI_GREEN_FLOWER_SHOOT_RIGHT 82650
#define ID_ANI_GREEN_FLOWER_SHOOT_LEFT_FACE_UP 82560
#define ID_ANI_GREEN_FLOWER_SHOOT_RIGHT_FACE_UP 82660
#define ID_ANI_GREEN_FLOWER_DIVE_LEFT	82700
#define ID_ANI_GREEN_FLOWER_DIVE_RIGHT 82750
#define ID_ANI_GREEN_FLOWER_DIVE_LEFT_FACE_UP	82710
#define ID_ANI_GREEN_FLOWER_DIVE_RIGHT_FACE_UP 82760

class CFlower :public CGameObject 
{
	float minY, maxY, ay;
	ULONGLONG aim_start, rest_start, die_start;
	BOOLEAN enableToShoot;
	CEffectCollision* eff_die;
	int type;
public:
	CFlower() 
	{ 
		minY = -1; 
		maxY = -1; 
		aim_start = 0;
		rest_start = 0;
		die_start = 0;
		enableToShoot = true;
		eff_die = NULL;
		type = -1;
		ay = FLOWER_GRAVITY;
	};
	CFlower(float x, float y, int type) :CGameObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->vy = 0;
		this->ay = 0;
		this->state = FLOWER_STATE_IN_TUBE;
		this->type = type;
		if (type == FLOWER_TYPE_SHOOTING_RED)
			minY = y - FLOWER_HEIGHT - 8.0f;
		else if (type == FLOWER_TYPE_SHOOTING_GREEN)
			minY = y - GREEN_FLOWER_HEIGHT - 10.0f;
		else
			minY = y - GREEN_FLOWER_HEIGHT;
		maxY = y + 3.0f;
		aim_start = 0;
		rest_start = 0;
		die_start = 0;
		enableToShoot = true;
		eff_die = NULL;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int para);
	void AimAndShoot();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void UpdateShootinFlower();
	void UpdateGreenCarniFlower();

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	//int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void SetFlowerMinY(float min_Y) { this->minY = min_Y; }

	int GetAniID_Red();
	int GetAniID_Carni();
	int GetAniID_Green();
	void HandleFlowerDeadEffect();

	CFireBullet* SetSpeedFireBullet(CFireBullet* bullet, CMario* mario);
};