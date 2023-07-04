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

	CCard* static_card1 = new CCard(2745.0f, GOT_A_CARD_Y - 3.0f);
	static_card1->SetState(mario->GetTypeOfCardCollected());
	static_card1->SetState(CARD_STATE_STATIC);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(static_card1);
	CHud::GetInstance()->SetAllowToRenderCard(); //You Got A Card -> Vẽ Card lên Hud
}