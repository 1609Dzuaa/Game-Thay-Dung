#pragma once
#include "GameObject.h"
#include "Flower.h"
#include "PlayScene.h"

#define TUBE_BBOX_WIDTH		33
#define TUBE_BBOX_HEIGHT	48
#define TUBE_CARNI_BBOX_HEIGHT	28

#define SHORT_TUBE_HAS_NO_FLOWER	0	//Chui lên 
#define TUBE_HAS_RED_FLOWER 1
#define TUBE_HAS_GREEN_FLOWER 2
#define TUBE_HAS_GREEN_CARNIVOURING_FLOWER 3
#define TUBE_HAS_2_STRIPE_BRICK 4
#define LONG_TUBE	5	//Chui xuống

#define ATTACK_ZONE 90.0f
#define SAFE_ZONE 45.0f
#define SAFE_ZONE_CARNI_FLOWER 31.0f

#define ID_ANI_TUBE 5500
#define ID_ANI_SMALL_TUBE 5501
#define ID_ANI_TUBE_WITH_STRIPE_BRICK 5502

#define DOWN_TUBE_POSITION_X	135
#define DOWN_TUBE_POSITION_Y	412

#define SHORT_TUBE_POSITION_X	2304
#define SHORT_TUBE_POSITION_Y	310

class CTube :public CGameObject 
{
	int Type; //Has shootin' flower, has Carni flower
	CFlower* flower = NULL; //Một cái ống thì chứa một loại hoa HOẶC 0
public:
	CTube(float x, float y, int type) :CGameObject(x, y) 
	{ 
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		this->Type = type; 
		flower = new CFlower(this->x, this->y, type);
		current_scene->AddObjectToScene(flower);
		//add nó vào scene nhưng vì Mario chưa lại gần Red_zone nên nó sẽ ở 
		//trạng thái wait -> chờ được vẽ và cập nhật lên
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects); //gọi hoa trong hàm update
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	void SpawnFlower();
	int GetType() { return this->Type; }
};