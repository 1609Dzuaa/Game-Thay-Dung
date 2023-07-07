#include "MarioWorld.h"
#include "GameObject.h"
#include "Entrance.h"
#include "Game.h"
#include "debug.h"

void CWorldMapPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	//DebugOutTitle(L"[debug] %d",testDebug);
	//CDataGame* data = CGame::GetInstance()->GetDataGame();
	//Cho game over
	/*if (data->GetIsContinue()) {
		data->ResetFullData();
		CGame::GetInstance()->InitiateSwitchScene(ID_SCENE_WORLD_MAP_RESET);
	}
	if (data->GetIsEnd()) {
		data->ResetFullData();
		CGame::GetInstance()->InitiateSwitchScene(ID_SCENE_INTRO);
	}
	//Tat display HUD
	if (data->GetIsDisplayHUD()) {
		if (GetTickCount64() - start_stop_hud_sub > TIME_DISPLAY_HUD_SUB) {
			data->SetIsDisplayHUD(false);
			start_stop_hud_sub = 0;
		}
	}*/
	//Neu dang di => khong the chuyen scene
	if (!CanActive()) isCanGoWorld = false;
	//Chuyen scene
	if (sceneChange && isCanGoWorld) {
		SaveData();
		CGame::GetInstance()->InitiateSwitchScene(sceneChange);
	}
	if (isGoingNodeX == true) {
		if (vx * (x - startX) >= 0) {
			x = startX;
			vx = 0;
			vy = 0;
			isGoingNodeX = false;
		}
	}
	/*why    vx* (x - startX)>0. Because more simple this code
	if (isGoingNodeX == true) {
		if (vx > 0) {
			if (x > startX) {
				x = startX;
				vx = 0;
				vy = 0;
				isGoingNodeX = false;
			}
		}
		else if(vx<0){
			if (x<startX) {
				x = startX;
				vx = 0;
				vy = 0;
				isGoingNodeX = false;
			}
		}
	}*/
	if (isGoingNodeY == true) {
		if (vy * (y - startY) >= 0) {
			y = startY;
			vx = 0;
			vy = 0;
			isGoingNodeY = false;
		}
	}
	//DebugOutTitle(L"vx vy: %f %f", vx, vy);
	//DebugOutTitle(L"x y: %f %f", x,y);
	//DebugOutTitle(L"x y startX startY: %f %f %f %f", x,y,startX,startY);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CWorldMapPlayer::Render() 
{
	/*CDataGame* data = CGame::GetInstance()->GetDataGame();

	int aniId = ID_ANI_MARIO_WORLD_MAP;
	if (data->GetLevel() == MARIO_LEVEL_SMALL) aniId = ID_ANI_MARIO_WORLD_MAP;
	else if (data->GetLevel() == MARIO_LEVEL_BIG) aniId = ID_ANI_MARIO_BIG_WORLD_MAP;
	else if (data->GetLevel() == MARIO_LEVEL_TAIL) aniId = ID_ANI_MARIO_TAIL_WORLD_MAP;
	else if (data->GetLevel() == MARIO_LEVEL_FIRE) aniId = ID_ANI_MARIO_FIRE_WORLD_MAP;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);*/
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_MARIO_SMALL_AT_WORLD;
	animations->Get(aniId)->Render(x, y, false);
}

void CWorldMapPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	top = y - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	right = left + MARIO_WORLD_MAP_BBOX_HEIGHT;
	bottom = top + MARIO_WORLD_MAP_BBOX_HEIGHT;

}

void CWorldMapPlayer::OnNoCollision(DWORD dt) 
{
	x += vx * dt;
	y += vy * dt;
}

void CWorldMapPlayer::OnCollisionWith(LPCOLLISIONEVENT e) 
{
	if (e->obj->IsBlocking()) {
		vx = 0;
		vy = 0;
	}
	if (dynamic_cast<CEntrance*>(e->obj))
		OnCollisionWithEntrance(e);
	//else if (dynamic_cast<COtherObject*>(e->obj))
		//OnCollisionWithOtherObject(e);
}

/*void CWorldMapPlayer::OnCollisionWithOtherObject(LPCOLLISIONEVENT e) {

	COtherObject* object = dynamic_cast<COtherObject*>(e->obj);
	sceneChange = 0;
	isAllowLeft = object->GetAllowLeft();
	isAllowRight = object->GetAllowRight();
	isAllowTop = object->GetAllowTop();
	isAllowBottom = object->GetAllowBottom();
	if (e->nx != 0) {
		Go1NodeX(object);
	}
	if (e->ny != 0) {
		Go1NodeY(object);
	}
}*/
void CWorldMapPlayer::OnCollisionWithEntrance(LPCOLLISIONEVENT e) 
{
	/*CEntrance* door = dynamic_cast<CEntrance*>(e->obj);
	isAllowLeft = door->GetAllowLeft();
	isAllowRight = door->GetAllowRight();
	isAllowTop = door->GetAllowTop();
	isAllowBottom = door->GetAllowBottom();
	sceneChange = door->GetIdScene();
	saveDoorProcess = door->GetModel();
	if (e->nx != 0) {
		Go1NodeX(door);
	}
	if (e->ny != 0) {
		Go1NodeY(door);
	}*/

}

void CWorldMapPlayer::SetState(int state) 
{
	switch (state) 
	{
	case MARIO_STATE_GO_LEFT:
		vy = 0.0f;
		vx = -SPEED_MARIO;
		break;
	case MARIO_STATE_GO_RIGHT:
		vy = 0.0f;
		vx = SPEED_MARIO;
		break;
	case MARIO_STATE_GO_TOP:
		vy = -SPEED_MARIO;
		vx = 0.0f;
		break;
	case MARIO_STATE_GO_UNDER:
		vy = SPEED_MARIO;
		vx = 0.0f;
		break;
	case MARIO_STATE_GO_WORLD_1:
		isCanGoWorld = true;
		break;
	}
}

void CWorldMapPlayer::Go1NodeX(LPGAMEOBJECT gameobject) 
{
	startX = gameobject->GetX();
	isGoingNodeX = true;
}

void CWorldMapPlayer::Go1NodeY(LPGAMEOBJECT gameobject) 
{
	startY = gameobject->GetY();
	isGoingNodeY = true;
}
void CWorldMapPlayer::SaveData() 
{
	/*CDataGame* data = CGame::GetInstance()->GetDataGame();
	data->SaveDoorProcess(saveDoorProcess);
	data->SavePositionXWorldMap(x);
	data->SavePositionYWorldMap(y);
	data->SaveAllowKey(isAllowLeft, isAllowTop, isAllowRight, isAllowBottom);*/
}