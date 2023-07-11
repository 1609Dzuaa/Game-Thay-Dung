#include "IntroKeyHandler.h"
#include "IntroPlayScene.h"
#include "RedArrow.h"
#include "debug.h"
#include "Scene.h"

//Ở Intro thì chỉ cần điều khiển mũi tên

void CIntroKeyEvent::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
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
			CGame::GetInstance()->InitiateSwitchScene(ID_WORLD);
		//Dựa vào level của Arrow
		break;
	}
}