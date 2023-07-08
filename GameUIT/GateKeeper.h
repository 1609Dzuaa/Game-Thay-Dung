#pragma once
#include "GameObject.h"

#define ID_ANI_GATE_KEEPER_MOVE_LEFT	3160	
#define ID_ANI_GATE_KEEPER_MOVE_RIGHT	3161

#define GATE_KEEPER_BBOX_HEIGHT	17
#define GATE_KEEPER_BBOX_WIDTH	17

//thực ra theo nguyên tác gọi là Hammer Brother
//nhưng đặt Kẻ Gác Cổng nghe cũng ngầu phết :D
//Nghiên cứu thêm hành động của nó, đôi lúc di chuyển từ điểm này tới điểm khác ?
class CGateKeeper : public CGameObject
{
	float initX;
public:
	CGateKeeper(float x, float y) :CGameObject(x, y) { this->vx = 0.014f; this->initX = x; };
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};