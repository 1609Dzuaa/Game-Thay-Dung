#pragma once
#include "GameObject.h"

#define ID_BLACK_SCREEN	255

#define BLACK_SCR_EFF_STATE_DRAW_FROM_1	1
#define BLACK_SCR_EFF_STATE_DRAW_FROM_0	0
#define BLACK_SCR_EFF_STATE_STOP_DRAW	-1

//Class này để vẽ black screen tạo hiệu ứng chuyển cảnh, tương đồng với RED BBox

class CEffect : public CGameObject 
{
	float Alpha; //Dùng để làm mờ
public:
	CEffect(float x, float y) { Alpha = -1; };
	CEffect(float x, float y, float _alpha) :CGameObject(x, y) 
	{ 
		Alpha = _alpha;
		if (Alpha == 0.0f)
			state = BLACK_SCR_EFF_STATE_DRAW_FROM_0;
		else if (Alpha == 1.0f)
			state= BLACK_SCR_EFF_STATE_DRAW_FROM_1;
		//state = -1; 
	}
	~CEffect() {};
	int IsBlocking() { return 0; };
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	void SetState(int state);
};