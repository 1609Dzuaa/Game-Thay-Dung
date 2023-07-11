#pragma once
#include "Scene.h"

class CIntroKeyEvent : public CSceneKeyHandler
{
public:
	virtual void OnKeyDown(int KeyCode);
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyUp(int KeyCode) {};
	CIntroKeyEvent(LPSCENE s) : CSceneKeyHandler(s) {}
};