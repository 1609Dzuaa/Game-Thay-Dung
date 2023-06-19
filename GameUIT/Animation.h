#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	BOOLEAN AffectedByStopWatch;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; AffectedByStopWatch = false; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, BOOLEAN affectBySW);
};

typedef CAnimation* LPANIMATION;