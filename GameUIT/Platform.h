#pragma once

#include "GameObject.h"
// 
// The most popular type of object in Mario! 
// 

#define TRADITIONAL_PLATFORM 1
#define COLOR_PLATFORM 2

class CPlatform : public CGameObject
{
protected:
	int length, type;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public:
	CPlatform(float x, float y, /*int type,*/
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->type = type;
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt) {}
	int IsBlocking() { return 1; } //Platform là loại Object đặc biệt
	//int IsCollidable() { return 1; }
	int GetType() { return type; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	//void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;