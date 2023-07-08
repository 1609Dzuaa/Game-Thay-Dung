#include "WorldKeyHandler.h"
#include "MarioWorld.h"
#include "WorldPlayScene.h"
#include "debug.h"

void CWorldMapKeyEvent::OnKeyDown(int KeyCode)
{
	CMarioWorld* mario = (CMarioWorld*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//Thêm phần Block phím khi đang move
	switch (KeyCode)
	{
	case DIK_RIGHT:
		if (!mario->GetIsMoving() && !mario->GetBlockDirect().z)
			mario->SetState(MARIO_WORLD_STATE_MOVE_RIGHT);
		break;
	case DIK_LEFT:
		if (!mario->GetIsMoving() && !mario->GetBlockDirect().x)
			mario->SetState(MARIO_WORLD_STATE_MOVE_LEFT);
		break;
	case DIK_UP:
		if (!mario->GetIsMoving() && !mario->GetBlockDirect().y)
			mario->SetState(MARIO_WORLD_STATE_MOVE_UP);
		break;
	case DIK_DOWN:
		if (!mario->GetIsMoving() && !mario->GetBlockDirect().w)
			mario->SetState(MARIO_WORLD_STATE_MOVE_DOWN);
		break;
	case DIK_S:
		if (!mario->GetIsMoving())
			mario->SetState(MARIO_WORLD_STATE_ENTER_ENTRANCE);
		break;
	}
}