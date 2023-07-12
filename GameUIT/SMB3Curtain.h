#pragma once
#include "GameObject.h"

//#define SMB3_CURTAIN_SPEED_UP	0.075f
#define SMB3_CURTAIN_SPEED_DOWN	0.1f

#define ID_ANI_SMB3_CURTAIN	500002

class CSMB3Curtain : public CGameObject
{
	BOOLEAN HasSpawned;
public:
	CSMB3Curtain(float x, float y) :CGameObject(x, y)
	{
		vy = SMB3_CURTAIN_SPEED_DOWN;
		HasSpawned = false;
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