#pragma once
#include "GameObject.h"

#define CARD_BBOX_WIDTH		16
#define CARD_BBOX_HEIGHT	16

#define CARD_STATE_MUSHROOM	0	//1350đ
#define CARD_STATE_STAR	1	//1450đ
#define CARD_STATE_FLOWER	2	//1500đ
#define CARD_STATE_BE_COLLECTED	3
#define CARD_STATE_STATIC	4	//Card nằm trong dòng chữ You Got A Card

#define ID_ANI_CARD_MUSHROOM	91000
#define ID_ANI_CARD_STAR	91001
#define ID_ANI_CARD_FLOWER	91002

#define ID_ANI_BLACK_CARD_MUSHROOM	91100
#define ID_ANI_BLACK_CARD_STAR	91101
#define ID_ANI_BLACK_CARD_FLOWER	91102

#define ID_ANI_STATIC_CARD_MUSHROOM	91200
#define ID_ANI_STATIC_CARD_STAR	91201
#define ID_ANI_STATIC_CARD_FLOWER	91202

#define TIME_SWITCH_CARD	150

#define CARD_FLY_SPEED		0.06f

//Card có 2 bug: 1 của Mario khiến nó 0 di chuyển, 1 là hiện Text quá nhanh
//Kh thấy card ở Hud ???

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
	void OnNoCollision(DWORD dt);
};