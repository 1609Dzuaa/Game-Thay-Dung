#include "MarioWorld.h"
#include "PlayScene.h" //Get Mario's level, coin, HP, Score, to Update World Hud
#include "GameObject.h"
#include "Entrance.h"
#include "BlockPoint.h"
#include "BlackScreen.h"
#include "Game.h"
#include "DataBinding.h"
#include "Hud.h"
#include "debug.h"

void CMarioWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (HasCollidedWithEntrance || HasCollidedWithBlock)
		HandlePositionWithEntranceAndBlock();

	if (state == MARIO_WORLD_STATE_ENTER_ENTRANCE)
	{
		if (Entrance_ID != 0) //tránh nhấn S khi Collide với Start Entrance
		{
			if (!init)
			{
				CBlackScreen::GetInstance()->SetState(BLACK_SCR_EFF_STATE_DRAW_FROM_0);
				init = 1;
			}
			//đợi nó đen toàn World rồi mới chuyển cảnh
			if (CBlackScreen::GetInstance()->GetAlpha() == 1.0f)
			{
				CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
				mario->SetIsAtWorld(0);
				int numEtrPass = CDataBindings::GetInstance()->NumEntrancePass;

				//Cập nhật x, y tại map
				CDataBindings::GetInstance()->WorldEntrance[numEtrPass].x = x;
				CDataBindings::GetInstance()->WorldEntrance[numEtrPass].y = y;

				CGame::GetInstance()->InitiateSwitchScene(Entrance_ID);
				atW = 0;
			}
		}
		return;
	}
	
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOutTitle(L"isAllow, atMW: %d, %d", isAllowToPlayThatEntrance, Entrance_ID);
	//DebugOut(L"CamX, CamY: %f, %f\n", CCamera::GetInstance()->GetCamPos().x, CCamera::GetInstance()->GetCamPos().y);	//DebugOutTitle(L"CardType: %d", mario->GetTypeOfCardCollected());
	DebugOutTitle(L"St, x, y: %d, %f, %f", state, x, y);
}

void CMarioWorld::HandlePositionWithEntranceAndBlock()
{
	if (vx > 0)
	{
		if (x - Entrance_Position.x >= 0)
		{
			x = Entrance_Position.x;
			vx = 0;
			HasCollidedWithEntrance = false;
			HasCollidedWithBlock = false;
			isMoving = false;
		}
	}
	else if (vx < 0)
	{
		if (x - Entrance_Position.x <= 0)
		{
			x = Entrance_Position.x;
			vx = 0;
			HasCollidedWithEntrance = false;
			HasCollidedWithBlock = false;
			isMoving = false;
		}
	}
	else if (vy > 0)
	{
		if (y - Entrance_Position.y >= 0)
		{
			y = Entrance_Position.y;
			vy = 0;
			HasCollidedWithEntrance = false;
			HasCollidedWithBlock = false;
			isMoving = false;
		}
	}
	else if (vy < 0)
	{
		if (y - Entrance_Position.y <= 0)
		{
			y = Entrance_Position.y;
			vy = 0;
			HasCollidedWithEntrance = false;
			HasCollidedWithBlock = false;
			isMoving = false;
		}
	}
	//Mục đích đưa nhân vật vào giữa Entrance dù Collided
}

void CMarioWorld::Render() 
{
	CAnimations* animations = CAnimations::GetInstance();
	
	int aniId = ID_ANI_MARIO_SMALL_AT_WORLD;
	animations->Get(aniId)->Render(x, y, false);
}

void CMarioWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	top = y - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	right = left + MARIO_WORLD_MAP_BBOX_HEIGHT;
	bottom = top + MARIO_WORLD_MAP_BBOX_HEIGHT;
}

void CMarioWorld::OnNoCollision(DWORD dt) 
{
	x += vx * dt;
	y += vy * dt;
}

void CMarioWorld::OnCollisionWith(LPCOLLISIONEVENT e) 
{
	if (dynamic_cast<CBlockPoint*>(e->obj))
		OnCollisionWithBlockPoint(e);
	if (dynamic_cast<CEntrance*>(e->obj))
		OnCollisionWithEntrance(e);
}

void CMarioWorld::OnCollisionWithBlockPoint(LPCOLLISIONEVENT e)
{
	CBlockPoint* blockPoint = dynamic_cast<CBlockPoint*>(e->obj);
	isAllowToPlayThatEntrance = false;
	isMoving = false;
	HasCollidedWithBlock = true;
	Entrance_Position.x = blockPoint->GetX();
	Entrance_Position.y = blockPoint->GetY();
	Direct_Been_Blocked = blockPoint->GetBlockDirect(); //Cập nhật hướng Block cho Mario
}

void CMarioWorld::OnCollisionWithEntrance(LPCOLLISIONEVENT e) 
{
	CEntrance* entr = dynamic_cast<CEntrance*>(e->obj);
	Entrance_ID = entr->GetSceneID();
	Entrance_Position.x = entr->GetX();
	Entrance_Position.y = entr->GetY();
	int numEtrPass = CDataBindings::GetInstance()->NumEntrancePass;
	CDataBindings::GetInstance()->WorldEntrance[numEtrPass].ID = entr->GetType();
	HasCollidedWithEntrance = true;

	if (IsAllowToEnterEntrance(entr))
		isAllowToPlayThatEntrance = true;

	Direct_Been_Blocked = entr->GetBlockDirect(); //Cập nhật hướng Block cho Mario
}

bool CMarioWorld::IsAllowToEnterEntrance(CEntrance* entr_para)
{
	//Nếu là Entrance Start thì return false luôn
	if (!entr_para->GetType()) return false;

	int numEtrPass = CDataBindings::GetInstance()->NumEntrancePass;
	for (int i = 0; i < numEtrPass; i++)
	{
		if (entr_para->GetType() == CDataBindings::GetInstance()->WorldEntrance[i].ID
			&& CDataBindings::GetInstance()->WorldEntrance[i].isPassed)
			return false;
	}
	return true;
}

void CMarioWorld::SetState(int state) 
{
	switch (state)
	{
	case MARIO_WORLD_STATE_MOVE_RIGHT:
		isMoving = true;
		vx = 0.09f;
		vy = 0; //Chỉ vận tốc di chuyển theo 1 trục
		break;

	case MARIO_WORLD_STATE_MOVE_LEFT:
		isMoving = true;
		vx = -0.09f;
		vy = 0;
		break;

	case MARIO_WORLD_STATE_MOVE_UP:
		isMoving = true;
		vy = -0.09f;
		vx = 0;
		break;

	case MARIO_WORLD_STATE_MOVE_DOWN:
		isMoving = true;
		vy = 0.09f;
		vx = 0;
		break;

	case MARIO_WORLD_STATE_ENTER_ENTRANCE:
		//if (!IsPassedThisEntrance(Entrance_Type))
			//isAllowToPlayThatEntrance = true;

		break;
	}
	CGameObject::SetState(state);
}

bool CMarioWorld::IsPassedThisEntrance(int entr_type)
{
	for (int i = 0; i < EntranceHasPassed.size(); i++)
		if (entr_type == EntranceHasPassed[i])
			return true;
	return false;
}