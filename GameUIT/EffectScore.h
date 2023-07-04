#pragma once
#include "Effect.h"

#define EFFECT_SCORE_VELO_Y 0.03f

#define NORMAL_SCORE 10
#define DOUBLE_SCORE 20
#define QUADRA_SCORE 40
#define DOUBLE_QUADRA_SCORE 80
#define ITEM_SCORE 100 //Mushroom, Leaf
#define ITEM_SCORE_LV_UP	110
#define COURSE_CLEAR_TEXT	120
#define GOT_A_CARD_TEXT		130

#define COURSE_CLEAR_X		2675.0f
#define COURSE_CLEAR_Y		130.0f

#define GOT_A_CARD_X		2675.0f
#define GOT_A_CARD_Y		175.0f

#define GOT_A_CARD_DELAY	400

#define STATE_READY_SPAWN_GOT_A_CARD	1

class CEffectScore :public CEffect 
{
	//coi dòng chữ là EffectScore
	float minY;
	int aniId;
	BOOLEAN AllowToDraw, isDelaying, isSpawnGotCard;
	ULONGLONG delay_start;
public:
	CEffectScore(float x, float y, float min_Y, int ani_ID) :CEffect(x, y) 
	{
		this->minY = min_Y;
		this->vy = -EFFECT_SCORE_VELO_Y;
		this->aniId = ani_ID;
		AllowToDraw = false;
		isDelaying = false;
		isSpawnGotCard = false;
		delay_start = 0;
		state = 0;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SpawnGotACard();
	void SetState(int state);
	void HandleSpawnGotACard();
	void SpawnCardType(); //dùng để vẽ card tĩnh cùng lúc với dòng Got A Card
};