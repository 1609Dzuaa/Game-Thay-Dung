#pragma once
#include "GameObject.h"

#define SMB3_CURTAIN_SPEED_DOWN	0.1f

#define SMB3_STATE_BLACK_BG	1
#define SMB3_STATE_COLOR_BG	2

#define ID_ANI_SMB3_CURTAIN_BLACK_BG	500002
#define ID_ANI_SMB3_CURTAIN_COLOR_BG	500003

class CSMB3Curtain : public CGameObject
{
	BOOLEAN HasSpawned;
public:
	CSMB3Curtain(float x, float y) :CGameObject(x, y)
	{
		vy = SMB3_CURTAIN_SPEED_DOWN;
		HasSpawned = false;
		state = SMB3_STATE_BLACK_BG;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
	void Render();
	void OnNoCollision(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state);

	void SpawnOtherObjects(); //star, leaf, mushroom, goomba, mai rùa xanh, đen
};