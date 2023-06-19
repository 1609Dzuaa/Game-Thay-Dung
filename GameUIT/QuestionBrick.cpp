#include "QuestionBrick.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "debug.h"
#include "PlayScene.h"
#include "FireBullet.h"

void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != QBRICK_STATE_HITTED)
		animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y, false);
	else 
		animations->Get(ID_ANI_QUESTION_BRICK_HITTED)->Render(x, y, false);
}

void CQuestionBrick::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (this->HasItem)
	{
		if (mario->GetLevel() != MARIO_LEVEL_SMALL) //level lớn thì cho lá
			this->type = HAS_LEAF;
		else //level nhỏ thì cho nấm
			this->type = HAS_MUSHROOM;
	}

	isEmpty = false;
	HandleBouncingBrick();
	if (isEmpty && this->type == HAS_MUSHROOM)
		SpawnMushroom(x, y);
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
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CCoin* coin = new CCoin(xBrick, yBrick, veloY);
	current_scene->AddObjectToScene(coin);
	coin->SetSpeed(0, -COIN_FLY_SPEED);
}

void CQuestionBrick::SpawnMushroom(float xBrick, float yBrick)
{
	if (!blockMushroom)
		mr->SetState(MUSHROOM_STATE_RISE_UP);
}

void CQuestionBrick::SpawnLeaf(float xBrick, float yBrick)
{
	blockMushroom = true;
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CLeaf* leaf = new CLeaf(xBrick, yBrick - QBRICK_BBOX_HEIGHT / 2);
	leaf->SetMinY(yBrick - QBRICK_BBOX_HEIGHT / 2 - 70.0f);
	current_scene->AddObjectToScene(leaf);
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
		if (this->type == HAS_LEAF)
			SpawnLeaf(x, y);
		else if(this->type == HAS_COIN)
			SpawnCoin(x, y, -COIN_FLY_SPEED);
		vy = -BOUNCING_SPEED;
		break;
	}

    CGameObject::SetState(state);
}