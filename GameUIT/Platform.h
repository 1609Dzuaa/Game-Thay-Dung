#pragma once

#include "GameObject.h"
// 
// The most popular type of object in Mario! 
// 

#define TRADITIONAL_PLATFORM 1
#define COLOR_PLATFORM 2

class CPlatform : public CGameObject
{
	int HasFlowerInside;
	int TypeOfFlower;
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 1; } //Platform là loại Object đặc biệt
	//int IsCollidable() { return 1; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	//void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;