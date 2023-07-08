#pragma once
#include "GameObject.h"

#define BLOCKPOINT_BBOX_WIDTH	5
#define BLOCKPOINT_BBOX_HEIGHT	5

class CBlockPoint :public CGameObject
{
	D3DXVECTOR4 BlockDirect;
public:
	CBlockPoint(float x, float y, float block_L, float block_T, float block_R, float block_B) :CGameObject(x, y)
	{
		BlockDirect.x = block_L;
		BlockDirect.y = block_T;
		BlockDirect.z = block_R;
		BlockDirect.w = block_B;
	}
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	//Lấy hướng Block: x, y, z, w lần lượt là L, T, R, B
	D3DXVECTOR4 GetBlockDirect() { return BlockDirect; }
};