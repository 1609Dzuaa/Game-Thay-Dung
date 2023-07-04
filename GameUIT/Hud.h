#pragma once
#include "Card.h"

#define ID_HUD	100000

#define CARD_UNDRAW_TIME	200

class CHud
{
	//là đối tượng tĩnh 0 chịu ảnh hưởng của Cam
	//Tương đồng với Cam, Map nên sẽ 0 kế thừa từ class nào
	static CHud* __HudInstance;	//Đảm bảo chỉ có duy nhất 1 HUD
	float x, y;
	int allowToRenderCard;	//Hiện dòng "You Got A Card" thì mới cho vẽ Card lên Hud
	BOOLEAN isUndrawInitialized;
	BOOLEAN untouch_0; // cờ 0 vẽ
	BOOLEAN untouch_1; // cờ vẽ
	ULONGLONG untouch_draw_0; //thgian 0 vẽ
	ULONGLONG untouch_draw_1; //thgian vẽ
public:
	CHud(float x, float y)
	{
		this->x = x;
		this->y = y;
		allowToRenderCard = 0;
		untouch_draw_0 = 0;
		untouch_draw_1 = 0;
		untouch_0 = 0;
		untouch_1 = 0;
		isUndrawInitialized = 0;
	}
	static CHud* GetInstance();
	void Update();
	void Render();
	void RenderCard();
	void SetAllowToRenderCard() { this->allowToRenderCard = 1; }
	void HandleCardDrawState();
};