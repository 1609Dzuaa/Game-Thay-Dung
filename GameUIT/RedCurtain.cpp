#include "RedCurtain.h"
#include "MarioNPC.h"
#include "LuigiNPC.h"
#include "IntroPlayScene.h"
#include "Goomba.h"
#include "debug.h"

void CRedCurtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Đi quá mốc nào đó thì Spawn 2 anh em
	if (y < -100 && !HasSpawned)
	{
		SpawnMarioAndLuigi();
		vy = 0;
		HasSpawned = true;
		this->Delete();
	}

	//DebugOutTitle(L"Y: %f", this->y);
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
	CMarioNPC* mario_npc = new CMarioNPC(x + 100, 170);
	CLuigiNPC* luigi_npc = new CLuigiNPC(x - 100, 170);
	CIntroPlayScene* current_scene = (CIntroPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(mario_npc);
	current_scene->AddObjectToScene(luigi_npc);
}