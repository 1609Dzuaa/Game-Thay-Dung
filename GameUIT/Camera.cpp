#include "Camera.h"
#include "Game.h"
#include "debug.h"
#include <cstddef>

CCamera* CCamera::pCam = NULL;

CCamera* CCamera::GetInstance()
{
    CGame* game = CGame::GetInstance();
    if (pCam == NULL)
    {
        pCam = new CCamera(game->GetBackBufferWidth(), game->GetBackBufferHeight());
        DebugOut(L"Create Camera Success\n");
    }
    return pCam;
}

CCamera::CCamera(int width, int height)
{
    this->Width = width;
    this->Height = height;

    this->posX = 0;
    this->posY = 0;

    //mFixedPos = NULL;
}

D3DXVECTOR2 CCamera::GetCamPos()
{
	return D3DXVECTOR2(posX, posY);
}

void CCamera::SetCamPos(float x, float y)
{
	this->posX = x;
	this->posY = y;
}