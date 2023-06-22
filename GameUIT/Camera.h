#pragma once
#include "D3DX10.h"
#include "GameObject.h"

class CCamera
{
	int Width, Height; //dài, rộng cam - cho = cửa sổ
	float posX, posY; //vị trí cam
	static CCamera* __Instance;
	LPGAMEOBJECT target_follow; //đối tượng mà muốn cam follow
public:
	CCamera() 
	{
		this->Width = 0;
		this->Height =0;
		this->posX = 0;
		this->posY = 0;
		this->target_follow = NULL;
	};
	CCamera(int width, int height);
	~CCamera() {};
	static CCamera* GetInstance(); 
	D3DXVECTOR2 GetCamPos(); //Lấy vị trí cam
	void SetCamPos(float x, float y); //Set vị trí mới cho cam
	void SetTargetToFollow(LPGAMEOBJECT obj); //Set đối tượng follow
	void Update();
	bool isViewable(LPGAMEOBJECT object); //Check obj có trong Cam không
};