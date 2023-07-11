#pragma once
#include "GameObject.h"
#include "DataBinding.h"

#define ID_GRASS	3150
#define ID_HELP		3151

#define DRAW_TIME	250

class CGrass :public CGameObject
{
	int isHelp; //Gộp chung với class này luôn
	BOOLEAN untouch_0; // cờ 0 vẽ
	BOOLEAN untouch_1; // cờ vẽ
	ULONGLONG untouch_draw_0; //thgian 0 vẽ
	ULONGLONG untouch_draw_1; //thgian vẽ
public:
	CGrass(float x, float y, int isHelp) :CGameObject(x, y)
	{
		this->isHelp = isHelp;
		if (isHelp)
		{
			untouch_1 = 1;
			untouch_draw_1 = GetTickCount64();
		}
		else
			untouch_1 = 0;
		untouch_0 = 0;
		untouch_draw_0 = 0;
		untouch_draw_1 = 0;
	};
	void Render() 
	{
		HandleHelpDrawState();

		if (untouch_0) return;

		CAnimations* animations = CAnimations::GetInstance();
		if (!isHelp)
			animations->Get(ID_GRASS)->Render(x, y, true);
		else
		{
			if (CDataBindings::GetInstance()->HP < 0) return; //0 vẽ Help nếu HP <0
			animations->Get(ID_HELP)->Render(x, y, true);
		}
	};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void HandleHelpDrawState() 
	{
		if (isHelp && GetTickCount64() - untouch_draw_0 >= DRAW_TIME && untouch_0)
		{
			untouch_0 = 0;
			untouch_1 = 1;
			untouch_draw_0 = 0;
			untouch_draw_1 = GetTickCount64();
		} //vẽ
		else if (isHelp && GetTickCount64() - untouch_draw_1 >= DRAW_TIME && untouch_1)
		{
			untouch_0 = 1;
			untouch_1 = 0;
			untouch_draw_0 = GetTickCount64();
			untouch_draw_1 = 0;
		}
	};
};