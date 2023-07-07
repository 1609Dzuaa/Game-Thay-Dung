#include "WorldKeyHandler.h"
#include "MarioWorld.h"
#include "WorldPlayScene.h"
#include "debug.h"

void CWorldMapKeyEvent::OnKeyDown(int KeyCode)
{
	CWorldMapPlayer* player = (CWorldMapPlayer*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_RIGHT:
		player->SetState(MARIO_WORLD_STATE_MOVE_RIGHT);
		break;
	}
}