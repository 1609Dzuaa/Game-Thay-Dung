#pragma once
#include "D3DX10.h"

class CCamera
{
	int Width, Height;
	float posX, posY;
	static CCamera* pCam;
public:
	CCamera() {};
	CCamera(int width, int height);
	~CCamera() {};
	static CCamera* GetInstance(); 
	D3DXVECTOR2 GetCamPos(); //Lấy vị trí cam
	void SetCamPos(float x, float y); //Set vị trí mới cho cam
};