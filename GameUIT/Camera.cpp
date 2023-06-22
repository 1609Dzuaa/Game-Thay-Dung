#include "Camera.h"
#include "Game.h"
#include "debug.h"
#include <cstddef>

CCamera* CCamera::__Instance = NULL;

CCamera* CCamera::GetInstance()
{
    CGame* game = CGame::GetInstance();
    if (__Instance == NULL)
    {
        __Instance = new CCamera(game->GetBackBufferWidth(), game->GetBackBufferHeight());
        DebugOut(L"Create Camera Success. Width: %d, Height: %d\n", game->GetBackBufferWidth(), game->GetBackBufferHeight());
    }
    return __Instance;
}

CCamera::CCamera(int width, int height)
{
    this->Width = width;
    this->Height = height;
    this->posX = 0;
    this->posY = 0;
    this->target_follow = NULL;
}

D3DXVECTOR2 CCamera::GetCamPos()
{
	return D3DXVECTOR2(posX, posY);
}

void CCamera::SetCamPos(float x, float y)
{
    this->posX = x - SCREEN_WIDTH / 2;
    this->posY = y - SCREEN_HEIGHT / 2;
    //để nhân vật nếu đi quá nửa màn hình thì move cam
}

void CCamera::SetTargetToFollow(LPGAMEOBJECT obj)
{
    this->target_follow = obj;
}

void CCamera::Update()
{
    if (!target_follow) return;

    //Set vị trí cho Cam
    SetCamPos(target_follow->GetX(), target_follow->GetY());

    //Giới hạn Left, Top cho Cam
    if (this->posX < 0) this->posX = 0; 
    if (this->posY < 0) this->posY = 0; 

    //Giới hạn Right, Bot cho Cam
    if (this->posX + SCREEN_WIDTH >= MAP1_1_WIDTH) this->posX = MAP1_1_WIDTH - SCREEN_WIDTH + 7.5f;
    if (this->posY + SCREEN_HEIGHT >= MAP1_1_HEIGHT) this->posY = MAP1_1_HEIGHT - SCREEN_HEIGHT;
}

bool CCamera::isViewable(LPGAMEOBJECT obj)
{
    float l, t, r, b;
    obj->GetBoundingBox(l, t, r, b);
    float obj_Width = r - l;
    float obj_Height = b - t;
 
    //Vì nó là 2D nên phải Thoả 4ĐK:
    if (obj->GetX() + obj_Width / 2 >= this->posX && obj->GetX() - obj_Width / 2 <= this->posX + this->Width &&
        obj->GetY() + obj_Height / 2 >= this->posY && obj->GetY() - obj_Height / 2 <= this->posY + this->Height)
    {
        return true;
    }
    return false;
}