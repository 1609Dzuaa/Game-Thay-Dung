#include "QuestionBrick.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "debug.h"

//extern list<LPGAMEOBJECT> objects;

void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != QBRICK_STATE_HITTED)
		animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y);
	else 
		animations->Get(ID_ANI_QUESTION_BRICK_HITTED)->Render(x, y);
	RenderBoundingBox();
}

void CQuestionBrick::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isEmpty = false;
	HandleBouncingBrick();
	if (isEmpty && this->type == HAS_MUSHROOM)
		SpawnMushroom(x, y);
	CGameObject::Update(dt,coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBrick::HandleBouncingBrick()
{
	if (y <= minY)
	{
		vy = BOUNCING_SPEED;
	}
	if (y > currentY)
	{
		y = currentY;
		vy = 0;
		isEmpty = true;
	}
}

void CQuestionBrick::SpawnCoin(float xBrick, float yBrick, float veloY)
{
	CCoin* coin = new CCoin(xBrick, yBrick, veloY);
	coin->SetSpeed(0, -COIN_FLY_SPEED);
	//objects.push_back(coin);
}

void CQuestionBrick::SpawnMushroom(float xBrick, float yBrick)
{
	CMushroom* mushroom = new CMushroom(xBrick, yBrick - QBRICK_BBOX_HEIGHT / 2 - 2.5f, 109.0f, GetTickCount64());
	mushroom->SetBrickMinY(minY);
	mushroom->SetMushroomX(x);
	//objects.push_back(mushroom);
}

void CQuestionBrick::SpawnLeaf(float xBrick, float yBrick)
{
	CLeaf* leaf = new CLeaf(xBrick, yBrick);
	leaf->SetMinY(y - 70.0f);
	//leaf->setMinX(x);
	//objects.push_back(leaf);
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBRICK_BBOX_WIDTH / 2;
	t = y - QBRICK_BBOX_HEIGHT / 2;
	r = l + QBRICK_BBOX_WIDTH;
	b = t + QBRICK_BBOX_HEIGHT;
}

void CQuestionBrick::SetState(int state)
{
	switch (state)
	{
	case QBRICK_STATE_HITTED:
		//SpawnCoin(x, y, -COIN_FLY_SPEED);
		//SpawnMushroom(x, y);
		if (this->type == HAS_LEAF)
			SpawnLeaf(x, y);
		vy = -BOUNCING_SPEED;
		break;
	}

    CGameObject::SetState(state);
}