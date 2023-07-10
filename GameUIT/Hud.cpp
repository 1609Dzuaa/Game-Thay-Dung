#include "Hud.h"
#include "Camera.h"
#include "PlayScene.h"
#include "WorldPlayScene.h"
#include "MarioWorld.h"
#include"DataBinding.h"

CHud* CHud::__HudInstance = NULL;
//int CHud::cardType[3] = { 0,0,0 };
Card CHud::cardCollected[3] = { 0,0,0 };
int CHud::numCardCollected = 0;

CHud* CHud::GetInstance()
{
	if (__HudInstance == NULL)
	{
		CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
		if (current_scene->GetID() != ID_WORLD)
		{
			float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
			float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() - 16.0f;
			__HudInstance = new CHud(x, y);
		}
		else
		{
			float x = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2 + 3.5f);
			float y = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2 + 35.0f);
			__HudInstance = new CHud(x, y);
		}
		DebugOut(L"Create Hud Successfully\n");
	}
	return __HudInstance;
}

void CHud::Update()
{
	//Update vị trí của Hud theo Cam
	//Chia ra vị trí ở MainWorld và Underground
	//CMarioWorld* mario_world = (CMarioWorld*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//if (mario_world->GetAtW())
	//{
		//for (int i = 0; i < numCardCollected; i++)
			//cardCollected[i].NoFlashAnymore = 1;
	this->x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
	this->y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() - 16.0f;
	//UpdateCard();
	//DebugOut(L"NumCard, C1, C2, C3: %d, %d, %d\n", numCardCollected, cardCollected[0].type, cardCollected[1].type, cardCollected[2].type);
	DebugOut(L"ID, isPass, NumPass: %d, %d, %d\n", CDataBindings::GetInstance()->WorldEntrance[0].ID, CDataBindings::GetInstance()->WorldEntrance[0].isPassed, CDataBindings::GetInstance()->NumEntrancePass);
}

void CHud::UpdateCard()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (numCardCollected == 3)
	{
		mario->HP += Get3Card();
		memset(cardCollected, 0, numCardCollected);
		numCardCollected = 0;
	}
}

int CHud::Get3Card()
{
	//Careful Warning here:
	//How about Create An Instance Class That Store Every Data From 1-1 And Bind It To WORLD ?
	if (cardCollected[0].type == cardCollected[1].type == cardCollected[2].type == CARD_STATE_STAR)
		return 5;
	else if (cardCollected[0].type == cardCollected[1].type == cardCollected[2].type == CARD_STATE_FLOWER)
		return 3;
	else if (cardCollected[0].type == cardCollected[1].type == cardCollected[2].type == CARD_STATE_MUSHROOM)
		return 2;
	else
		return 1;
}

void CHud::Render()
{
	//Không nên set các giá trị != 0 sau dấu . (VD: 3.5f) tại vị trí vẽ vì
	//đôi lúc nó sẽ bị lệch do 0 theo kịp
	CAnimations* animations = CAnimations::GetInstance();
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
	CPlayScene* play_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (play_scene->GetWait() && current_scene->GetID() != ID_WORLD)
		RenderPauseText();

	//Vẽ thanh Hud
	animations->Get(ID_HUD)->Render(x, y, false);
	//Vẽ World number -> vì world hiện tại là 1 nên mặc định vẽ tại đây luôn
	animations->Get(ID_NUMBER_1)->Render(x - 77.0f, y - 3.0f, false);
	//Vẽ M ;)
	animations->Get(ID_LETTER_M)->Render(x - 106.0f, y + 5.0f, false);
	RenderHP();
	RenderCoin();
	RenderTimer();
	RenderPoints();
	RenderSpeedBar(); //prob here
	RenderCard(); //Có vấn đề: Lâu lâu lại 0 hiện ?!!!
}

void CHud::RenderHP()
{
	//Bug đêm qua -1 là do chết quá số mạng nên nó 0 tìm thấy frame - 1 xDDDDDDD
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(mario->GetHP())->Render(x - 77.0f, y + 5.0f, false);
}

void CHud::RenderCoin()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	//if <= 9 : vẽ 1 số
	//Quay về mấy bài NMLT năm nhất, tìm chữ số hàng đơn vị, hàng chục, đảo ngc số, ...
	//else CT chung: Hàng đơn vị = GetCoin() % 10, Hàng chục = GetCoin() / 10
	if (mario->GetCoin() <= 9)
		animations->Get(mario->GetCoin())->Render(x + 25.0f, y - 3.0f, false);
	else
	{
		int	OnesPlace = mario->GetCoin() % 10;	//Hàng đơn vị
		int TensPlace = mario->GetCoin() / 10;	//Hàng chục
		animations->Get(OnesPlace)->Render(x + 26.0f, y - 3.0f, false);
		animations->Get(TensPlace)->Render(x + 18.0f, y - 3.0f, false);
	}
}

void CHud::RenderPoints()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	int points = mario->GetPoints();

	int	OnesPlace = points % 10;	//Hàng đơn vị
	int TensPlace = (points / 10) % 10;	//Hàng chục
	int Hundreds = (points / 100) % 10;	//Hàng trăm
	int Thousands = (points / 1000) % 10;	//Hàng ngàn
	int	TenThousands = (points / 10000) % 10;	//Hàng chục ngàn
	int HundredThousands = (points / 100000) % 10;	//Hàng trăm ngàn
	int Millions = points / 1000000;	//Hàng triệu

	animations->Get(OnesPlace)->Render(x - 18.0f, y + 5.0f, false);
	animations->Get(TensPlace)->Render(x - 25.0f, y + 5.0f, false);
	animations->Get(Hundreds)->Render(x - 32.0f, y + 5.0f, false);
	animations->Get(Thousands)->Render(x - 39.0f, y + 5.0f, false);
	animations->Get(TenThousands)->Render(x - 46.0f, y + 5.0f, false);
	animations->Get(HundredThousands)->Render(x - 53.0f, y + 5.0f, false);
	animations->Get(Millions)->Render(x - 60.0f, y + 5.0f, false);
}

void CHud::RenderSpeedBar()
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
	//Không có đoạn If này thì khi ở World diagnostic lên đến vài GB ;)
	//1 of Major bug's been solved
	if (current_scene->GetID() == ID_WORLD) 
		return;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	
	//Bar
	for (int i = 1; i <= mario->GetSpeedBar(); i++)
	{
		for (int j = 1; j <= mario->GetSpeedBar(); j++)
		{
			//Vẽ đc 6 thằng trên thì mới vẽ thằng này (Undraw như untouchable)
			if (j == 7)animations->Get(ID_P_BUTTON)->Render(x - 9.0f, y - 4.0f, false);
			else animations->Get(ID_SPEED_BAR)->Render(x - (62 - 8 * (j - 1)), y - 4.0f, false);
		}
	}
}

void CHud::RenderTimer()
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	if (current_scene->GetID() != ID_WORLD)
	{
		CAnimations* animations = CAnimations::GetInstance();
		CPlayScene* current_scene_1 = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		int	OnesPlace = current_scene_1->GetTimer() % 10;	//Hàng đơn vị
		int TensPlace = (current_scene_1->GetTimer() / 10) % 10;	//Hàng chục
		int Hundreds = current_scene_1->GetTimer() / 100;	//Hàng trăm

		animations->Get(OnesPlace)->Render(x + 26.0f, y + 5.0f, false);
		animations->Get(TensPlace)->Render(x + 18.0f, y + 5.0f, false);
		animations->Get(Hundreds)->Render(x + 10.0f, y + 5.0f, false);
	}
	else
	{
		CAnimations* animations = CAnimations::GetInstance();
		for (int i = 0; i < 3; i++)
			animations->Get(ID_NUMBER_0)->Render(x + 10.0f + i * 8.0f, y + 5.0f, false);
	}
}

void CHud::RenderCard()
{
	CAnimations* animations = CAnimations::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CMarioWorld* mario_world = (CMarioWorld*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//chúng ta sẽ 0 return ở đây mà thêm thuộc tính cho struct Card
	//đó là AllowToRender, trong khi vẽ chúng ta sẽ Loop 3 vòng Card
	//Check nếu đc Allow thì vẽ nó, 0 thì bỏ qua
	//Đã xong, còn lỗi Ăn 2 Card giống nhau liên tiếp thì nó increase 3 card:
	//vd: vừa ăn xong star => qua scene kế ăn tiếp star thì cho 3 card và vẽ 3 card ???!!
	//Đã XONG TH ăn 2 Card liên tiếp!!

	//Tuy nhiên: vấn đề mới là xem xét lại hàm này vì có lẽ vi phạm quy tắc

	if (mario->GetIsAtMainWorld()) //Vẽ Card ở Map 1-1 . if(isaffected)=>vẽ chớp cho nó
	{
		float x1 = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() - 69.0f;

		for (int i = 0; i < numCardCollected; i++)
		{
			if (cardCollected[i].isInitUndraw && !cardCollected[i].NoFlashAnymore)
				HandleCardDrawState(cardCollected[i]);
			else 
			{
				cardCollected[i].draw = 1;
				untouch_draw_1 = GetTickCount64();
				cardCollected[i].isInitUndraw = 1;
				//Khởi tạo hiệu ứng chớp chớp cho Card
			}
			if (cardCollected[i].aniID != 0 && cardCollected[i].isAllowToRender && cardCollected[i].draw)
				animations->Get(cardCollected[i].aniID)->Render(x1 + 24 * i, 372.0f, false);
		}
	}
	else if (mario_world->GetAtW()) //Vẽ Card ở World
	{
		for (int i = 0; i < numCardCollected; i++)
			if (cardCollected[i].aniID != 0)
			{
				animations->Get(cardCollected[i].aniID)->Render(x + 54.0f + i * 24.0f, y, false);
				cardCollected[i].NoFlashAnymore = 1;
			}
	}
}

void CHud::SetTypeCardAndAniID(int para, int type)
{
	cardCollected[para].type = type;

	if (cardCollected[para].type == CARD_STATE_MUSHROOM)
		cardCollected[para].aniID = ID_ANI_STATIC_CARD_MUSHROOM;
	else if (cardCollected[para].type == CARD_STATE_STAR)
		cardCollected[para].aniID = ID_ANI_STATIC_CARD_STAR;
	else if (cardCollected[para].type == CARD_STATE_FLOWER)
		cardCollected[para].aniID = ID_ANI_STATIC_CARD_FLOWER;
}

void CHud::HandleCardDrawState(Card& card_para)
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

int CHud::GetAniIDCard()
{
	return 0;
}

void CHud::RenderPauseText()
{
	CAnimations* animations = CAnimations::GetInstance();
	float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
	float y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() / 2;
	animations->Get(ID_LETTER_P)->Render(x, y, false);
	animations->Get(ID_LETTER_A)->Render(x + 10.0f, y, false);
	animations->Get(ID_LETTER_U)->Render(x + 20.0f, y, false);
	animations->Get(ID_LETTER_S)->Render(x + 30.0f, y, false);
	animations->Get(ID_LETTER_E)->Render(x + 40.0f, y, false);
}