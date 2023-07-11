#include "WorldKeyHandler.h"
#include "MarioWorld.h"
#include "WorldPlayScene.h"
#include "RedArrow.h"
#include "debug.h"
#include "Scene.h"

void CWorldMapKeyEvent::OnKeyDown(int KeyCode)
{
	CMarioWorld* mario = (CMarioWorld*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//Thêm phần Block phím khi đang move HOẶC đang enter entrance
	if (CDataBindings::GetInstance()->HP < 0)
	{
		switch (KeyCode)
		{
		case DIK_UP:
			CRedArrow::GetInstance()->SetLevel(0);
			break;

		case DIK_DOWN:
			CRedArrow::GetInstance()->SetLevel(1);
			break;
			
		case DIK_S:
			if (!CRedArrow::GetInstance()->GetLevel())
			{
				//Chuyển Scene thì Reset hết các Data
				//Sáng mai dậy nhớ Check kĩ các biến static tồn tại trong suốt
				//quá trình chạy CT
				CDataBindings::GetInstance()->RefreshAllData();
				CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();
				
				if (current_scene->GetID() == ID_WORLD)
					CGame::GetInstance()->InitiateSwitchScene(ID_WORLD_SUB);
				else 
					CGame::GetInstance()->InitiateSwitchScene(ID_WORLD);
			}
			//Dựa vào level của Arrow
			break;
		}
		return; //Chết quá 5 mạng thì chỉ cho phép bấm 3 phím này
	}

	switch (KeyCode)
	{
	case DIK_RIGHT:
		if (mario->GetState() == MARIO_WORLD_STATE_ENTER_ENTRANCE) return;

		if (!mario->GetIsMoving() && !mario->GetBlockDirect().z)
			mario->SetState(MARIO_WORLD_STATE_MOVE_RIGHT);
		break;

	case DIK_LEFT:
		if (mario->GetState() == MARIO_WORLD_STATE_ENTER_ENTRANCE) return;

		if (!mario->GetIsMoving() && !mario->GetBlockDirect().x)
			mario->SetState(MARIO_WORLD_STATE_MOVE_LEFT);
		break;

	case DIK_UP:
		if (mario->GetState() == MARIO_WORLD_STATE_ENTER_ENTRANCE) return;

		if (!mario->GetIsMoving() && !mario->GetBlockDirect().y)
			mario->SetState(MARIO_WORLD_STATE_MOVE_UP);
		break;

	case DIK_DOWN:
		if (mario->GetState() == MARIO_WORLD_STATE_ENTER_ENTRANCE) return;

		if (!mario->GetIsMoving() && !mario->GetBlockDirect().w)
			mario->SetState(MARIO_WORLD_STATE_MOVE_DOWN);
		break;

	case DIK_S:
		if (mario->GetState() == MARIO_WORLD_STATE_ENTER_ENTRANCE) return;
		
		if (!mario->GetIsMoving() && mario->GetIsAllowEnter())
			mario->SetState(MARIO_WORLD_STATE_ENTER_ENTRANCE);
		break;
	}
}