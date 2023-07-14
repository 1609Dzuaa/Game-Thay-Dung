#include "Hud.h"
#include "Camera.h"
#include "PlayScene.h"
#include "WorldPlayScene.h"
#include "MarioWorld.h"
#include"DataBinding.h"

CHud* CHud::__HudInstance = NULL;
BOOLEAN CHud::isStarting = 0;
BOOLEAN CHud::initStart = 0;
BOOLEAN CHud::isAllowToRenderHudStart = 0;
ULONGLONG CHud::Hud_Start_Draw_Time = 0;

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
	this->x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() / 2;
	this->y = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() - 16.0f;
	UpdateCard();
	//DebugOut(L"ID, isPass, NumPass: %d, %d, %d\n", CDataBindings::GetInstance()->WorldEntrance[0].ID, CDataBindings::GetInstance()->WorldEntrance[0].isPassed, CDataBindings::GetInstance()->NumEntrancePass);
}

void CHud::UpdateCard()
{
	if (CDataBindings::GetInstance()->numCardCollected == 3)
	{
		CDataBindings::GetInstance()->HP += Get3Card();
		memset(CDataBindings::GetInstance()->cardCollected, 0, CDataBindings::GetInstance()->numCardCollected);
		CDataBindings::GetInstance()->numCardCollected = 0;
	}
}

int CHud::Get3Card()
{
	//Careful Warning here: DONE!
	//"unsafe operation: no value of type 'bool' promoted to type 'int' can equal the given constant"
	//if(a == b == 3) NO 
	//=> Solution: if(a == 3 && b == 3) 
	//How about Create An Instance Class That Store Every Data From 1-1 And Bind It To WORLD ?
	if (CDataBindings::GetInstance()->cardCollected[0].type == CARD_STATE_STAR
		&& CDataBindings::GetInstance()->cardCollected[1].type == CARD_STATE_STAR
		&& CDataBindings::GetInstance()->cardCollected[2].type == CARD_STATE_STAR)
		return 5;
	else if (CDataBindings::GetInstance()->cardCollected[0].type == CARD_STATE_FLOWER
		&& CDataBindings::GetInstance()->cardCollected[1].type == CARD_STATE_FLOWER
		&& CDataBindings::GetInstance()->cardCollected[2].type == CARD_STATE_FLOWER)
		return 3;
	else if (CDataBindings::GetInstance()->cardCollected[0].type == CARD_STATE_MUSHROOM
		&& CDataBindings::GetInstance()->cardCollected[1].type == CARD_STATE_MUSHROOM
		&& CDataBindings::GetInstance()->cardCollected[2].type == CARD_STATE_MUSHROOM)
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
	if (play_scene->GetWait() && current_scene->GetID() != ID_WORLD && current_scene->GetID() != ID_WORLD_SUB)
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
	RenderCard(); 

	if (isAllowToRenderHudStart)
		RenderHudStart();
	
	if (CDataBindings::GetInstance()->HP < 0)
		RenderHudEnd();
}

void CHud::RenderHP()
{
	//Bug đêm qua -1 là do chết quá số mạng nên nó 0 tìm thấy frame - 1 xDDDDDDD
	//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int current_HP = CDataBindings::GetInstance()->HP;
	if (current_HP < 0)
		return;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(current_HP)->Render(x - 77.0f, y + 5.0f, false);
}

void CHud::RenderCoin()
{
	CAnimations* animations = CAnimations::GetInstance();
	//if <= 9 : vẽ 1 số
	//Quay về mấy bài NMLT năm nhất, tìm chữ số hàng đơn vị, hàng chục, đảo ngc số, ...
	//else CT chung: Hàng đơn vị = GetCoin() % 10, Hàng chục = GetCoin() / 10
	if (CDataBindings::GetInstance()->coin <= 9)
		animations->Get(CDataBindings::GetInstance()->coin)->Render(x + 25.0f, y - 3.0f, false);
	else
	{
		int	OnesPlace = CDataBindings::GetInstance()->coin % 10;	//Hàng đơn vị
		int TensPlace = CDataBindings::GetInstance()->coin / 10;	//Hàng chục
		animations->Get(OnesPlace)->Render(x + 26.0f, y - 3.0f, false);
		animations->Get(TensPlace)->Render(x + 18.0f, y - 3.0f, false);
	}
}

void CHud::RenderPoints()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	int points = CDataBindings::GetInstance()->points;

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
	//Solved vấn đề vẽ quá nhiều SpeedBar
	//Do cái World dự phòng
	if (current_scene->GetID() != ID_MAP_1_1) //Chỉ vẽ SpeedBar khi ở Map1-1
		return;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CAnimations* animations = CAnimations::GetInstance();
	
	//Bar
	for (int i = 1; i <= mario->GetSpeedBar(); i++)
	{
		//Vẽ đc 6 thằng trên thì mới vẽ thằng này (Undraw như untouchable)
		if (i == 7)
			animations->Get(ID_P_BUTTON)->Render(x - 9.0f, y - 4.0f, false);
		else
			animations->Get(ID_SPEED_BAR)->Render(x - (62 - 8 * (i - 1)), y - 4.0f, false);
	}
}

void CHud::RenderTimer()
{
	CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	if (current_scene->GetID() != ID_WORLD)
	{
		CAnimations* animations = CAnimations::GetInstance();
		int	OnesPlace = CPlayScene::timer % 10;	//Hàng đơn vị
		int TensPlace = (CPlayScene::timer / 10) % 10;	//Hàng chục
		int Hundreds = CPlayScene::timer / 100;	//Hàng trăm

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


	if (mario_world->GetAtW()) //Vẽ Card ở World
	{
		for (int i = 0; i < CDataBindings::GetInstance()->numCardCollected; i++)
			if (CDataBindings::GetInstance()->cardCollected[i].aniID != 0)
			{
				animations->Get(CDataBindings::GetInstance()->cardCollected[i].aniID)->Render(x + 54.0f + i * 24.0f, y, false);
				CDataBindings::GetInstance()->cardCollected[i].NoFlashAnymore = 1;
			}
	}
	else //Vẽ Card ở Map 1-1
	{
		float x1 = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() - 69.0f;

		for (int i = 0; i < CDataBindings::GetInstance()->numCardCollected; i++)
		{
			if (CDataBindings::GetInstance()->cardCollected[i].isInitUndraw 
				&& !CDataBindings::GetInstance()->cardCollected[i].NoFlashAnymore)
				CDataBindings::GetInstance()->HandleCardDrawState(CDataBindings::GetInstance()->cardCollected[i]);
			else
			{
				CDataBindings::GetInstance()->cardCollected[i].draw = 1;
				CDataBindings::GetInstance()->cardCollected[i].isInitUndraw = 1;
				//Khởi tạo hiệu ứng chớp chớp cho Card
			}

			if (CDataBindings::GetInstance()->cardCollected[i].aniID != 0 
				&& CDataBindings::GetInstance()->cardCollected[i].isAllowToRender 
				&& CDataBindings::GetInstance()->cardCollected[i].draw)
			{
				float y_draw_pos = CCamera::GetInstance()->GetCamPos().y + CGame::GetInstance()->GetBackBufferHeight() / 2 + 130.0f;
				animations->Get(CDataBindings::GetInstance()->cardCollected[i].aniID)->Render(x1 + 24 * i, y_draw_pos, false);
			}
		}
	}
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

void CHud::RenderHudStart()
{
	CAnimations* animations = CAnimations::GetInstance();
	float x_draw_pos = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2);
	float y_draw_pos = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) - 77.0f;
	animations->Get(ID_HUD_START)->Render(x_draw_pos, y_draw_pos, false);
	animations->Get(ID_NUMBER_1)->Render(x_draw_pos + 20.0f, y_draw_pos - 10.0f, false);
	animations->Get(ID_NUMBER_4)->Render(x_draw_pos + 40.0f, y_draw_pos + 10.0f, false);
}

void CHud::RenderHudEnd()
{
	CAnimations* animations = CAnimations::GetInstance();
	float x_draw_pos = static_cast<float>(CGame::GetInstance()->GetBackBufferWidth() / 2);
	float y_draw_pos = static_cast<float>(CGame::GetInstance()->GetBackBufferHeight() / 2) - 77.0f;
	animations->Get(ID_HUD_END)->Render(x_draw_pos, y_draw_pos, false);
}