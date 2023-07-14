#include "DataBinding.h"
#include "Entrance.h"
#include "Card.h"
#include "Hud.h"
#include "PlayScene.h"
#include "WorldPlayScene.h"

CDataBindings* CDataBindings::__DBInstance = nullptr;

Entrance CDataBindings::WorldEntrance[7] = { -1,0 };
int CDataBindings::NumEntrancePass = 0;
Card CDataBindings::cardCollected[3] = { 0,0,0 };
int CDataBindings::numCardCollected = 0;
int CDataBindings::HP = 0;
int CDataBindings::coin = 0;
int CDataBindings::points = 0;
int CDataBindings::TypeOfCardCollected = 0;
int CDataBindings::IsStopWatch = 0;
int CDataBindings::IsCanPlay = 0;
int CDataBindings::timer = 300;
int CDataBindings::Mario_Level = 3; //Tiny

CDataBindings* CDataBindings::GetInstance()
{
	if (!__DBInstance)
		__DBInstance = new CDataBindings();
	return __DBInstance;
}

void CDataBindings::SetTypeCardAndAniID(int para, int type)
{
	cardCollected[para].type = type;

	if (cardCollected[para].type == CARD_STATE_MUSHROOM)
		cardCollected[para].aniID = ID_ANI_STATIC_CARD_MUSHROOM;
	else if (cardCollected[para].type == CARD_STATE_STAR)
		cardCollected[para].aniID = ID_ANI_STATIC_CARD_STAR;
	else if (cardCollected[para].type == CARD_STATE_FLOWER)
		cardCollected[para].aniID = ID_ANI_STATIC_CARD_FLOWER;
}

void CDataBindings::HandleCardDrawState(Card& card_para)
{
	if (GetTickCount64() - card_para.undraw_time >= CARD_UNDRAW_TIME && card_para.undraw)
	{
		card_para.undraw = 0;
		card_para.draw = 1;
		card_para.undraw_time = 0;
		card_para.draw_time = GetTickCount64();
	} //vẽ
	else if (GetTickCount64() - card_para.draw_time >= CARD_UNDRAW_TIME && card_para.draw)
	{
		card_para.undraw = 1;
		card_para.draw = 0;
		card_para.undraw_time = GetTickCount64();
		card_para.draw_time = 0;
	}
	//Kẹp tham chiếu cho nó vì ta muốn giá trị thuộc tính của Card thay đổi sau khi gọi hàm
}

void CDataBindings::RefreshAllData()
{
	//Đang có bug chết ở màn 1 mà chưa qua thì khi Continue 0 move đc
	//Solved!!! Chỉnh lại Cam đôi chỗ
	//Hiện đang có bug từ intro vào world rồi vào 1-1, chết xong load lại
	//thì diagnostic lên đến vài GB ??@@
	memset(WorldEntrance, 0, NumEntrancePass);
	memset(cardCollected, 0, numCardCollected);
	CDataBindings::NumEntrancePass = 0;
	CDataBindings::numCardCollected = 0;
	CDataBindings::HP = 0;
	CDataBindings::coin = 0;
	CDataBindings::points = 0;
	CDataBindings::TypeOfCardCollected = 0;
	CDataBindings::IsStopWatch = 0;
	CDataBindings::IsCanPlay = 0;
	CDataBindings::Mario_Level = 1;
	CPlayScene::timer = 300;
	CWorldPlayScene::initStartHud = 0; //Chỉ cho phép hiện Start Hud khi Load lại màn chơi từ đầu
	CHud::isStarting = 0;
	CHud::Hud_Start_Draw_Time = 0;
}