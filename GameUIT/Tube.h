#pragma once
#include "GameObject.h"
#include "ShootingFlower.h"
#include "PlayScene.h"

#define TUBE_BBOX_WIDTH		31
#define TUBE_BBOX_HEIGHT	47

#define TUBE_TYPE_SHOOTING_FLOWER 1


#define TUBE_STATE_SPAWN_FLOWER 1
#define TUBE_STATE_STOP_SPAWN 2

#define ATTACK_ZONE 70.0f

#define ID_ANI_TUBE 5500

class CTube :public CGameObject 
{
	int Type; //Has shootin' flower, has Cani flower
	BOOLEAN Flower_Arise;
	CShootingFlower* shoot_flower; //Một cái ống thì chứa một loại hoa
public:
	CTube(float x, float y, int type) :CGameObject(x, y) 
	{ 
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		this->Type = type; 
		this->Flower_Arise = false;
		shoot_flower = new CShootingFlower(this->x, this->y - TUBE_BBOX_HEIGHT / 2 - 1.5f);
		current_scene->AddObjectToScene(shoot_flower); 
		//add nó vào scene nhưng vì Mario chưa lại gần Red_zone nên nó sẽ ở 
		//trạng thái wait -> chờ được vẽ và cập nhật lên
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects); //gọi hoa trong hàm update
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	void SpawnShootingFlower();
	void SetState(int state);
	void SetArise(bool para) { this->Flower_Arise = para; }
};