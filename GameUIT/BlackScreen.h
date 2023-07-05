#pragma once

#define ID_BLACK_SCREEN	255

#define BLACK_SCR_EFF_STATE_DRAW_FROM_1	1
#define BLACK_SCR_EFF_STATE_DRAW_FROM_0	0
#define BLACK_SCR_EFF_STATE_STOP_DRAW	-1

//Class này để vẽ black screen tạo hiệu ứng chuyển cảnh, tương đồng với RED BBox

class CBlackScreen
{
	int state;
	float x, y;
	float Alpha; //Dùng để làm mờ
	static CBlackScreen* __BlackScrInstance;
public:
	static CBlackScreen* GetInstance();
	CBlackScreen(float x, float y)
	{
		state = BLACK_SCR_EFF_STATE_STOP_DRAW;
		this->x = x;
		this->y = y;
		this->Alpha = 0.0f;
	}
	~CBlackScreen()
	{
		if (__BlackScrInstance == nullptr) return;
		CBlackScreen* tempBlack = __BlackScrInstance;
		delete tempBlack;
		__BlackScrInstance = nullptr;
	}
	void Update();
	void Render();
	void SetState(int state);
};