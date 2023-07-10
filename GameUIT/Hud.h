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

#define CARD_UNDRAW_TIME	200

struct Card
{
	int type;
	int aniID;
	int isAllowToRender;//Check cho phép vẽ Card lên Hud khi hiện dòng "You Got ..." cuối Game
	int draw; //2 biến này dùng để vẽ card chớp 
	int undraw; //2 biến này dùng để vẽ card chớp
	int isInitUndraw;
	int NoFlashAnymore; //Để nhận biết card này 0 chớp đc nữa
	ULONGLONG undraw_time;
	ULONGLONG draw_time;
};

class CHud
{
	//Tương đồng với Cam, Map nên sẽ 0 kế thừa từ class nào
	static CHud* __HudInstance;	//Đảm bảo chỉ có duy nhất 1 HUD
	float x, y;
	int allowToRenderCard;	//Hiện dòng "You Got A Card" thì mới cho vẽ Card lên Hud
	//static int numCardCollected;
	static Card cardCollected[3];
	//static int cardType[3];  //mảng Card lượm đc
	BOOLEAN isUndrawInitialized;
	BOOLEAN isAffect; //Card vừa lượm đc(vẽ chớp chớp ở Hud)
	BOOLEAN initCard; //Check xem đã khởi tạo thông tin cho Card chưa (mỗi Card 1 lần)
	BOOLEAN isStarting;
	BOOLEAN AllowRenderHudEnd;
	ULONGLONG Hud_Start_Draw_Time;
public:
	static int numCardCollected;
	static BOOLEAN isAllowToPlay; //Cho phép chơi (khi đã hết hiện thanh Hud Start)
	CHud(float x, float y)
	{
		this->x = x;
		this->y = y;
		allowToRenderCard = 0;
		Hud_Start_Draw_Time = GetTickCount64();
		isUndrawInitialized = 0;
		isAffect = 0;
		isStarting = 1;
		initCard = 0;
		AllowRenderHudEnd = 0;
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

	void SetAllowToRenderCard() { cardCollected[numCardCollected].isAllowToRender = 1; }

	//Dùng để set các thuộc tính cho Card Hud khi nhặt Card ở cuối Game
	void SetTypeCardAndAniID(int para, int type);

	void IncreaseNumCardCollected() { numCardCollected++; }

	void HandleCardDrawState(Card& card_para);
	void RenderPauseText();
	void SetInitCard(BOOLEAN para) { this->initCard = para; }
	int GetAniIDCard();
};