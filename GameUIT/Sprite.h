#pragma once

#include "Texture.h"
#include "Game.h"

#define SHAKING_TIME	240
#define SHAKE_TIME	40
#define	BLACK_TRANSITION_TIME	1000

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;
	float alpha;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;

	ULONGLONG shake_start, shake_stop;
	BOOLEAN Shake, noShake, Initialized;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	void Draw(float x, float y);

	void StartShaking() { shake_start = GetTickCount64(); Shake = 1; Initialized = 1; }

	void HandleShaking();
};

typedef CSprite* LPSPRITE;