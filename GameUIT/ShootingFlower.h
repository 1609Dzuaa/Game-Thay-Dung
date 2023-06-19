#pragma once
#include "GameObject.h"
#include "FireBullet.h"
#include "PlayScene.h"
#include "EffectCollision.h"

//Quite similiar to Mushroom
//Chui từ vật thể ra

#define ATTACK_RANGE 90.0f
#define SHOOTING_FLOWER_RISE_SPEED 0.03f

#define SHOOTING_FLOWER_REST_TIME 1500
#define SHOOTING_FLOWER_AIM_TIME 500

#define FLOWER_WIDTH 16
#define FLOWER_HEIGHT 32

#define SHOOTING_FLOWER_STATE_IN_TUBE -1 //Chờ được vẽ và cập nhật
#define SHOOTING_FLOWER_STATE_DIVE 0 //dive tới một đoạn nhất định trong ống
#define SHOOTING_FLOWER_STATE_RISE_UP 1 //Đc Vẽ và cập nhật
#define SHOOTING_FLOWER_STATE_SHOOT 2
#define SHOOTING_FLOWER_STATE_REST 3
#define SHOOTING_FLOWER_STATE_DIE 4

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

class CShootingFlower :public CGameObject 
{
	float minY, maxY;
	ULONGLONG aim_start, rest_start, die_start;
	BOOLEAN enableToShoot;
	CEffectCollision* eff_die;
	int LastFrame;
public:
	CShootingFlower() 
	{ 
		minY = -1; 
		maxY = -1; 
		aim_start = 0;
		rest_start = 0;
		die_start = 0;
		enableToShoot = true;
		eff_die = NULL;
		LastFrame = 0;
	};
	CShootingFlower(float x, float y) :CGameObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->vy = 0;
		this->state = SHOOTING_FLOWER_STATE_IN_TUBE;
		minY = y - FLOWER_HEIGHT - 8.0f;
		maxY = y;
		aim_start = 0;
		rest_start = 0;
		die_start = 0;
		enableToShoot = true;
		eff_die = NULL;
		LastFrame = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int para);
	void AimAndShoot();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(DWORD dt);
	int IsBlocking() { return 0; };
	void SetFlowerMinY(float min_Y) { this->minY = min_Y; }

	int GetAniID();
	void HandleFlowerDeadEffect();

	CFireBullet* SetSpeedFireBullet(CFireBullet* bullet, CMario* mario);
};