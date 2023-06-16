#pragma once
#include "Platform.h"
class CColorPlatform : public CGameObject
{
	//Class này sẽ có một chút đặc biệt so với class Platform
	//Thay vì truyền vào 3 spriteID như bthg, ta sẽ truyền các giá trị 0 cho nó
	//Để hàm Render của class này sẽ KHÔNG VẼ các sprite đó ra mà chỉ LẤY BoundingBox của nó để phục vụ Collision
	//=> Mục đích để tránh việc hoạt ảnh Mario "đâm xuyên" qua các Color Platform do thứ tự Render
	//Cũng tránh đc việc Render Mario sau cùng vì thứ tự vẽ có thể ảnh hưởng đến tính toán Collision
	//Giả dụ vẽ 2 lớp Layer: lớp nào đc vẽ sau thì sẽ "block" lớp trước
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public:
	CColorPlatform(float x, float y,
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

	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	float GetCellWidth() { return cellWidth; }
	float GetCellHeight() { return cellHeight; }
	int GetLength() { return length; }
};