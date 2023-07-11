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
#define ID_HUD_START	100001
#define ID_HUD_END	100002

class CHud
{
	//Tương đồng với Cam, Map nên sẽ 0 kế thừa từ class nào
	static CHud* __HudInstance;	//Đảm bảo chỉ có duy nhất 1 HUD
	float x, y;
	BOOLEAN isUndrawInitialized;
	BOOLEAN isAffect; //Card vừa lượm đc(vẽ chớp chớp ở Hud)
	BOOLEAN initCard; //Check xem đã khởi tạo thông tin cho Card chưa (mỗi Card 1 lần)
	BOOLEAN isStarting;
	ULONGLONG Hud_Start_Draw_Time;
public:
	
	CHud(float x, float y)
	{
		this->x = x;
		this->y = y;
		Hud_Start_Draw_Time = GetTickCount64();
		isUndrawInitialized = 0;
		isAffect = 0;
		isStarting = 1;
		initCard = 0;
	}
	static CHud* GetInstance();
	void Update();
	//Xem xét việc tạo các hàm Update của từng đối tượng trên Hud
	//Rồi dựa vào đó mà Render tương ứng
	void UpdateCard();
	int Get3Card();

	void Render();
	void RenderHP();
	void RenderCoin();
	void RenderTimer();
	void RenderPoints();
	void RenderSpeedBar();
	void RenderCard();
	void RenderHudStart();
	void RenderHudEnd();

    void RenderPauseText();
	void SetInitCard(BOOLEAN para) { this->initCard = para; }
	int GetAniIDCard();
};