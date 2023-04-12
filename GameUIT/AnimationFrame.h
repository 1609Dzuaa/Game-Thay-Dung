#pragma once

#include "Sprite.h"

/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;
	//Class chuyển động khung hình gồm 2 thuộc tính là một con trỏ sprite để handle cái sprite cần chuyển động
	//và một biến thời gian cho chuyển động đó

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;