#include "WorldKeyHandler.h"
#include "MarioWorld.h"
#include "WorldPlayScene.h"
#include "debug.h"

void CWorldMapKeyEvent::OnKeyDown(int KeyCode)
{
	CWorldMapPlayer* player = (CWorldMapPlayer*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//CDataGame* data = CGame::GetInstance()->GetDataGame();
	/*if (data->GetIsDisplayHUD()) return;
	switch (KeyCode)
	{
	case DIK_RIGHT:
		if (!data->GetIsOver()) { if (player->CanActive() && player->GetAllowRight()) player->SetState(MARIO_STATE_GO_RIGHT); }
		break;
	case DIK_LEFT:
		if (!data->GetIsOver()) { if (player->CanActive() && player->GetAllowLeft()) player->SetState(MARIO_STATE_GO_LEFT); }
		break;
	case DIK_UP:
		if (!data->GetIsOver()) { if (player->CanActive() && player->GetAllowTop()) player->SetState(MARIO_STATE_GO_TOP); }
		else data->ChangeBetweenPrepareContinueAndEnd();
		break;
	case DIK_DOWN:
		if (!data->GetIsOver()) { if (player->CanActive() && player->GetAllowBottom()) player->SetState(MARIO_STATE_GO_UNDER); }
		else data->ChangeBetweenPrepareContinueAndEnd();
		break;
	case DIK_S:
		if (!data->GetIsOver()) { if (player->CanActive()) player->SetState(MARIO_STATE_GO_WORLD_1); }
		else if (data->GetIsPrepareContinue()) data->SettingCountinue();
		else if (data->GetIsPrepareEnd()) data->SettingEnd();
		break;
	}*/
}