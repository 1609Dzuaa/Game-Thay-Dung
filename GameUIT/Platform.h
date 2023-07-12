#pragma once

#include "GameObject.h"
// 
// The most popular type of object in Mario! 
// 

#define PLATFORM_TYPE_NORMAL	0
#define PLATFORM_UDW_TUBE	1

class CPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int isUnderworldTube;	//Vì vấn đề Sprite nên ống chui lên sẽ có kiểu này

public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, int _isUdwTube) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->isUnderworldTube = _isUdwTube;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 1; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetType() { return this->isUnderworldTube; }
	//void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;