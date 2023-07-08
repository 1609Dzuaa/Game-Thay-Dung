#include "Hud.h"
#include "Camera.h"
#include "PlayScene.h"
CHud* CHud::__HudInstance = NULL;

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
			float x = CGame::GetInstance()->GetBackBufferWidth() / 2;
			float y = CGame::GetInstance()->GetBackBufferHeight() - 90.0f;
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
	RenderSpeedBar();
	RenderCard(); //Có vấn đề: Lâu lâu lại 0 hiện ?!!!
}

void CHud::RenderHP()
{
	//Coi lại một chút
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
	CAnimations* animations = CAnimations::GetInstance();
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	int	OnesPlace = current_scene->GetTimer() % 10;	//Hàng đơn vị
	int TensPlace = (current_scene->GetTimer() / 10) % 10;	//Hàng chục
	int Hundreds = current_scene->GetTimer() / 100;	//Hàng trăm

	animations->Get(OnesPlace)->Render(x + 26.0f, y +5.0f, false);
	animations->Get(TensPlace)->Render(x + 18.0f, y + 5.0f, false);
	animations->Get(Hundreds)->Render(x + 10.0f, y + 5.0f, false);
}

void CHud::RenderCard()
{
	CAnimations* animations = CAnimations::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!allowToRenderCard) return;

	HandleCardDrawState();
	if (untouch_0) return;

	int aniId = -1;
	if (mario->GetTypeOfCardCollected() == CARD_STATE_MUSHROOM)
		aniId = ID_ANI_STATIC_CARD_MUSHROOM;
	else if (mario->GetTypeOfCardCollected() == CARD_STATE_STAR)
		aniId = ID_ANI_STATIC_CARD_STAR;
	else if (mario->GetTypeOfCardCollected() == CARD_STATE_FLOWER)
		aniId = ID_ANI_STATIC_CARD_FLOWER;

	if (aniId == -1) return;

	float x = CCamera::GetInstance()->GetCamPos().x + CGame::GetInstance()->GetBackBufferWidth() - 75.0f;
	animations->Get(aniId)->Render(x, 372.0f, false);

	//Khởi tạo hiệu ứng chớp chớp cho Card
	if (!isUndrawInitialized)
	{
		untouch_0 = 1;
		untouch_draw_0 = GetTickCount64();
		isUndrawInitialized = 1;
	}
}

void CHud::HandleCardDrawState()
{
	if (GetTickCount64() - untouch_draw_0 >= CARD_UNDRAW_TIME && untouch_0)
	{
		untouch_0 = 0;
		untouch_1 = 1;
		untouch_draw_0 = 0;
		untouch_draw_1 = GetTickCount64();
	} //vẽ
	else if (GetTickCount64() - untouch_draw_1 >= CARD_UNDRAW_TIME && untouch_1)
	{
		untouch_0 = 1;
		untouch_1 = 0;
		untouch_draw_0 = GetTickCount64();
		untouch_draw_1 = 0;
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