#pragma once
#include "Scene.h"

class CWorldMapKeyEvent : public CSceneKeyHandler
{
public:
	virtual void OnKeyDown(int KeyCode);
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyUp(int KeyCode) {};
	CWorldMapKeyEvent(LPSCENE s) : CSceneKeyHandler(s) {}
};