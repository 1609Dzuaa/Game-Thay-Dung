#pragma once
#include "Animations.h"

#define ID_ANI_RED_ARROW	100003	

class CRedArrow
{
	float x, y;
	int level; //Ban đầu = 0 tức nó trỏ tới Continue
public:
	static CRedArrow* __RedArrowInstance;
	static CRedArrow* GetInstance();
	CRedArrow(float x, float y) 
	{
		this->x = x;
		this->y = y;
		this->level = 0;
	};
	int GetLevel() { return this->level; }
	void SetLevel(int para);
	void Update(); //Update vị trí tuỳ vào Scene hiện tại
	void Render();
};