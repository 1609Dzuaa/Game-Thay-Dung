#include "Camera.h"
#include "Game.h"
#include "PlayScene.h"
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

CCamera::~CCamera()
{
    delete __Instance;
    __Instance = nullptr;
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
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (!target_follow) 
        return;
    if (mario->GetState() == MARIO_STATE_DIE)
    {
        this->posX = 0;
        this->posY = 0;
        return;
    }

    //Khi Enter Map 1-1 thì pos Cam lúc này là (0,96) dẫn đến việc khi chết
    //World bị lệch trên 1 khúc => lúc chết thì set lại vị trí cam thành (0,0)
    //Để World khỏi bị dời hình 

    //Set vị trí cho Cam
    SetCamPos(target_follow->GetX(), target_follow->GetY());

    //nếu đang ở Underworld thì đánh cờ điều chỉnh cam
    //CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (mario->GetIsAtMainWorld())
    {
        //Giới hạn Left, Top cho Cam
        if (this->posX < 0) this->posX = 0;
        if (this->posY < 0) this->posY = 0;

        //Giới hạn Right, Bot cho Cam
        if (this->posX + SCREEN_WIDTH >= MAP1_1_WIDTH) this->posX = MAP1_1_WIDTH - SCREEN_WIDTH + 7.5f;
        if (this->posY + SCREEN_HEIGHT >= MAP1_1_HEIGHT) this->posY = MAP1_1_HEIGHT - SCREEN_HEIGHT;
    }
    else
    {
        //Với Underworld thì chỉ cần giới hạn Left, Top, Right cho nó
        if (this->posX < 0) this->posX = 0;
        if (this->posY < MAP1_1_HEIGHT + 40.0f) this->posY = MAP1_1_HEIGHT + 40.0f; //Thêm bớt 1 đoạn vì nó ở dưới lòng đất

        if (this->posX + SCREEN_WIDTH >= UNDERWORLD_WIDTH) this->posX = UNDERWORLD_WIDTH - SCREEN_WIDTH + 7.5f;
    }
}

bool CCamera::isViewable(LPGAMEOBJECT obj)
{
    float l, t, r, b;
    obj->GetBoundingBox(l, t, r, b);
    float obj_Width = r - l;
    float obj_Height = b - t;
 
    //Vì nó là 2D nên phải Thoả 4ĐK:
    if (obj->GetX() + obj_Width / 2 > this->posX && obj->GetX() - obj_Width / 2 < this->posX + this->Width &&
        obj->GetY() + obj_Height / 2 > this->posY && obj->GetY() - obj_Height / 2 < this->posY + this->Height)
    {
        return true;
    }
    return false;
}