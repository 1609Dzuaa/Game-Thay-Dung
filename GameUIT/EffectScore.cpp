#include "EffectScore.h"
#include "PlayScene.h"
#include "Card.h"
#include "Hud.h"

void CEffectScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->aniId != COURSE_CLEAR_TEXT && this->aniId != GOT_A_CARD_TEXT)
	{
		if (this->y <= minY)
			this->Delete();
		else
			y += vy * dt;
	}
	else
	{
		if (this->aniId == COURSE_CLEAR_TEXT)
			HandleSpawnGotACard();
	}
	//DebugOutTitle(L"Delay: %d", isDelaying);
}

void CEffectScore::Render()
{
	if (aniId == COURSE_CLEAR_TEXT && !AllowToDraw) return;	//Delay vẽ Course Clear
	if (aniId == GOT_A_CARD_TEXT && isDelaying) return;
	if (IsWaiting && IsWaitable)
	{
		//2 dòng chữ cuối Game đc phép Render dù bấm W
		if (this->aniId != COURSE_CLEAR_TEXT && this->aniId != GOT_A_CARD_TEXT)
			return;
	}

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(this->aniId)->Render(x, y, false);
}

void CEffectScore::SetState(int state)
{
	switch (state)
	{
	case STATE_READY_SPAWN_GOT_A_CARD:
		delay_start = GetTickCount64();
		isDelaying = true;
		break;
	}

	CGameObject::SetState(state);
}

void CEffectScore::HandleSpawnGotACard()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetIsReachEndPos() && !isSpawnGotCard)
	{
		isSpawnGotCard = true;
		AllowToDraw = true;
		SetState(STATE_READY_SPAWN_GOT_A_CARD);
	}
	if (GetTickCount64() - delay_start > GOT_A_CARD_DELAY && isDelaying)
	{
		SpawnGotACard();
		isDelaying = false;
		delay_start = 0;
	}
}

void CEffectScore::SpawnGotACard()
{
	CEffectScore* eff = new CEffectScore(GOT_A_CARD_X, GOT_A_CARD_Y, 0, GOT_A_CARD_TEXT);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(eff);
	SpawnCardType();
}

void CEffectScore::SpawnCardType()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//Vì card cũ đi quá màn hình, bị xoá nên tạo card mới, 1 nằm ở chỗ You Got, 2 ở Hud
	CCard* static_card = new CCard(2745.0f, GOT_A_CARD_Y - 3.0f);
	static_card->SetState(mario->GetTypeOfCardCollected()); 
	static_card->SetState(CARD_STATE_STATIC);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(static_card);

	//Set cardType và Ani  1, 2, 3 vưa nhận đc và tăng số card lượm đc
	CHud::GetInstance()->SetTypeCardAndAniID(CHud::GetInstance()->numCardCollected, mario->GetTypeOfCardCollected());
	CHud::GetInstance()->SetAllowToRenderCard(); //cho Phép vẽ card đó lên Hud
	CHud::GetInstance()->IncreaseNumCardCollected(); //sau đó tăng số card thu đc lên 1
}