#pragma once
#include "GameObject.h"

#define CARD_BBOX_WIDTH		16
#define CARD_BBOX_HEIGHT	16

#define CARD_STATE_MUSHROOM	0
#define CARD_STATE_STAR	1
#define CARD_STATE_FLOWER	2
#define CARD_STATE_BE_COLLECTED	3

#define ID_ANI_CARD_STAR	91001
#define ID_ANI_CARD_FLOWER	91002

#define TIME_SWITCH_CARD	150

class CCard : public CGameObject
{
	ULONGLONG switch_card_start;
	BOOLEAN isMushroom, isStar, isFlower;
public:
	CCard(float x, float y) :CGameObject(x, y) 
	{ 
		switch_card_start = -1; 
		state = CARD_STATE_STAR; 
		isMushroom = 0;
		isStar = 1;
		isFlower = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
};