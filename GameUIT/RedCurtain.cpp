#include "RedCurtain.h"
#include "MarioNPC.h"
#include "LuigiNPC.h"
#include "IntroPlayScene.h"
#include "debug.h"

void CRedCurtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Đi quá mốc nào đó thì Spawn 2 anh em
	if (y < -100 && !HasSpawned)
	{
		SpawnMarioAndLuigi();
		vy = 0;
		HasSpawned = true;
	}

	DebugOutTitle(L"Y: %f", this->y);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedCurtain::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CRedCurtain::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_RED_CURTAIN)->Render(x, y, false);
}

void CRedCurtain::SetState(int state)
{

}

void CRedCurtain::SpawnMarioAndLuigi()
{
	CIntroPlayScene* current_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMarioNPC* mario_npc = new CMarioNPC(x + 50, 160);
	CLuigiNPC* luigi_npc = new CLuigiNPC(x - 150, 160);
	current_scene->AddObjectToScene(mario_npc);
	current_scene->AddObjectToScene(luigi_npc);
}