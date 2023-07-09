#pragma once
#include "Card.h"

#define ID_HUD	100000
#define ID_NUMBER_0	0
#define ID_NUMBER_1	1
#define ID_NUMBER_2	2
#define ID_NUMBER_3	3
#define ID_NUMBER_4	4
#define ID_NUMBER_5	5
#define ID_NUMBER_6	6
#define ID_NUMBER_7	7
#define ID_NUMBER_8	8
#define ID_NUMBER_9	9
#define ID_LETTER_M	13
#define ID_LETTER_P	16
#define ID_LETTER_A	17
#define ID_LETTER_U	18
#define ID_LETTER_S	19
#define ID_LETTER_E	22
#define ID_SPEED_BAR 30
#define ID_P_BUTTON	31

#define CARD_UNDRAW_TIME	200

struct Card
{
	int type;
	int aniID;
	int isInit;
};

class CHud
{
	//Tương đồng với Cam, Map nên sẽ 0 kế thừa từ class nào
	static CHud* __HudInstance;	//Đảm bảo chỉ có duy nhất 1 HUD
	float x, y;
	int allowToRenderCard;	//Hiện dòng "You Got A Card" thì mới cho vẽ Card lên Hud
	static int numCardCollected;
	static Card cardCollected[3];
	//static int cardType[3];  //mảng Card lượm đc
	BOOLEAN isUndrawInitialized;
	BOOLEAN untouch_0; // cờ 0 vẽ
	BOOLEAN untouch_1; // cờ vẽ
	BOOLEAN isAffect; //Card vừa lượm đc(vẽ chớp chớp ở Hud)
	BOOLEAN initCard;
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
		isAffect = 0;
		initCard = 0;
	}
	static CHud* GetInstance();
	void Update();
	void Render();
	void RenderHP();
	void RenderCoin();
	void RenderTimer();
	void RenderPoints();
	void RenderSpeedBar();
	void RenderCard();
	void SetAllowToRenderCard() { this->allowToRenderCard = 1; }
	void HandleCardDrawState();
	void RenderPauseText();
	void SetInitCard(BOOLEAN para) { this->initCard = para; }
};