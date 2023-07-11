#pragma once
#include "GameObject.h"
#include "DataBinding.h"
#include "Entrance.h"

#define SPEED_MARIO 0.1f
#define MARIO_WORLD_MAP_BBOX_WIDTH 8
#define MARIO_WORLD_MAP_BBOX_HEIGHT 8

#define ID_ANI_MARIO_SMALL_AT_WORLD 1795
#define ID_ANI_MARIO_BIG_AT_WORLD	1070
#define ID_ANI_MARIO_RACOON_AT_WORLD 2970

#define MARIO_WORLD_STATE_MOVE_RIGHT	100
#define MARIO_WORLD_STATE_MOVE_LEFT	200
#define MARIO_WORLD_STATE_MOVE_UP	300
#define MARIO_WORLD_STATE_MOVE_DOWN	400
#define MARIO_WORLD_STATE_ENTER_ENTRANCE	500

class CMarioWorld : public CGameObject
{
	BOOLEAN isAllowToPlayThatEntrance;
	BOOLEAN HasCollidedWithEntrance, HasCollidedWithBlock;
	BOOLEAN isMoving;
	vector<int> EntranceHasPassed;
	int Entrance_ID;
	int CardType;
	int atW;
	D3DXVECTOR2 Entrance_Position;
	D3DXVECTOR4 Direct_Been_Blocked; //Nếu chưa qua entrance thì entrance đó block hết các hướng còn lại
	BOOLEAN isTravelling;
	BOOLEAN init;
public:
	CMarioWorld(float x, float y) : CGameObject(x, y)
	{
		isAllowToPlayThatEntrance = false;
		HasCollidedWithEntrance = false; //Để nhận biết có thể chuyển Scene
		HasCollidedWithBlock = false;
		isMoving = false;
		Entrance_Position.x = 0;
		Entrance_Position.y = 0;
		Entrance_ID = 0;
		//Kh nên đặt Block ở đây vì có TH finish Map 1 
		//nhưng kh move bot đc do khởi tạo block ở đây :Đ
		//Ban đầu vị trí ở Start(Block L, T, B) -> obviously
		//Như vậy là khi chết rồi refresh lại nó 0 vào if
		if (!CDataBindings::GetInstance()->IsCanPlay)
		{
			Direct_Been_Blocked.x = 1;
			Direct_Been_Blocked.y = 1;
			Direct_Been_Blocked.z = 0;
			Direct_Been_Blocked.w = 1;
		}
		else
		{
			int numEntr = CDataBindings::GetInstance()->NumEntrancePass;
			Direct_Been_Blocked.x = CDataBindings::GetInstance()->WorldEntrance[numEntr - 1].BlockDirectL;
			Direct_Been_Blocked.y = CDataBindings::GetInstance()->WorldEntrance[numEntr - 1].BlockDirectT;
			Direct_Been_Blocked.z = CDataBindings::GetInstance()->WorldEntrance[numEntr - 1].BlockDirectR;
			Direct_Been_Blocked.w = CDataBindings::GetInstance()->WorldEntrance[numEntr - 1].BlockDirectB;
		}
		isTravelling = 0;
		init = 0;
		CardType = 0;
		atW = 1;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	int GetCardType() { return this->CardType; }
	int GetAtW() { return this->atW; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithEntrance(LPCOLLISIONEVENT e);
	void OnCollisionWithBlockPoint(LPCOLLISIONEVENT e);
	void HandlePositionWithEntranceAndBlock(); //Xử lý vị trí của Mario với Entrance và Block
	void SetCardType(int para) { this->CardType = para; }
	void SetAtW(int atw) { this->atW = atw; }
	void SetEntranceData(CEntrance* entr); //Dùng để Set data cho Entrance ở DataBindings
	bool IsPassedThisEntrance(int entr_type);
	void SetState(int state);
	bool GetIsMoving() { return this->isMoving; }
	bool GetIsAllowEnter() { return this->isAllowToPlayThatEntrance; }

	//Hàm này để check xem có thể chơi map đó kh
	bool IsAllowToEnterEntrance(CEntrance* entr_para);
	D3DXVECTOR4 GetBlockDirect() { return this->Direct_Been_Blocked; }
};