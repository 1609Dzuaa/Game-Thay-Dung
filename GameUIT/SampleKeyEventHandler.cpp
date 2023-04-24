#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Brick.h"

extern CMario* mario; 
//extern CFireBullet* bullet;
extern vector<LPGAMEOBJECT> objects;
CGame* game = CGame::GetInstance();

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_X:
		CFireBullet* bullet = new CFireBullet();
		objects.push_back(bullet);
		//Tạo viên đạn mới và push_back nó vào vector kiểu con trỏ CGameObject
		if (mario->GetRight())
			bullet->Fire(0); //bắn đạn về phía phải
		else
			bullet->Fire(1); //về phía trái
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{

	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetRight(true);
		mario->SetLeft(false);
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetRight(false);
		mario->SetLeft(true);
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);

	// Sitting state has higher priority 
	if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_SIT);
	}

}