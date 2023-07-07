#include "WorldKeyHandler.h"
#include "MarioWorld.h"
#include "WorldPlayScene.h"
#include "debug.h"

void CWorldMapKeyEvent::OnKeyDown(int KeyCode)
{
	CWorldMapPlayer* player = (CWorldMapPlayer*)((LPWORLDPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
}