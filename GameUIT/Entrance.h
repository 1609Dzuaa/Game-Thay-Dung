#pragma once
#include "GameObject.h"

#define ID_ANI_ENTRANCE_START	110000
#define ID_ANI_ENTRANCE_MAP_1	110001
#define ID_ANI_ENTRANCE_MAP_2	110002
#define ID_ANI_ENTRANCE_MAP_3	110003
#define ID_ANI_ENTRANCE_MAP_4	110004
#define ID_ANI_ENTRANCE_MAP_5	110005
#define ID_ANI_ENTRANCE_MAP_6	110006
#define ID_ANI_ENTRANCE_MAP_7	110007
#define ID_ANI_ENTRANCE_MAP_M	110008

#define ENTRANCE_BBOX_WIDTH		15
#define ENTRANCE_BBOX_HEIGHT	15

#define ENTRANCE_STATE_BEING_PASSED	1

class CEntrance :public CGameObject 
{
	int type, scene_id; //Similiar to Portal
	D3DXVECTOR4 BlockDirect;
public:
	CEntrance(float x, float y, int type, int scene_ID, float block_L, float block_T, float block_R, float block_B) :CGameObject(x, y)
	{
		this->type = type;
		this->scene_id = scene_ID;

		BlockDirect.x = block_L;
		BlockDirect.y = block_T;
		BlockDirect.z = block_R;
		BlockDirect.w = block_B;
		state = 0;
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int GetType() { return this->type; }
	int IsBlocking() { return 0; }
	int GetSceneID() { return this->scene_id; }
	//Lấy hướng Block: x, y, z, w lần lượt là L, T, R, B
	D3DXVECTOR4 GetBlockDirect() { return this->BlockDirect; }
	void SetState(int state);
};