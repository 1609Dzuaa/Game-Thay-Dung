#include "Brick.h"
#include "QuestionBrick.h"
#include "PlayScene.h"
#include "Switch.h"
#include "Coin.h"

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	UpdatePosition(dt);
	UpdateSpawnCoin();

	if (state == GOLD_BRICK_STATE_IS_HITTED && !IsDeleted())
	{
		eff = new CEffectCollision(x, min_pos - 5.0f, hit_start, EFF_COL_TYPE_SMOKE_EVOLVE);
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		current_scene->AddObjectToScene(eff);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//Gold brick lúc vỡ ra cũng bị ảnh hưởng bởi StopWatch
}

void CBrick::UpdatePosition(DWORD dt)
{
	if (y < min_pos)
	{
		vy = GOLD_BRICK_BOUNCING_SPEED;
	}
	if (y > old_pos)
	{
		//if it has switch, got hitted && at max pos => spawn switch
		if (state == GOLD_BRICK_STATE_IS_HITTED)
			SpawnSwitch();
		y = old_pos;
		vy = 0;
	}
}

void CBrick::UpdateSpawnCoin()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetIsHitSwitch() && !this->hasSwitch && type == GOLD_BRICK)
	{
		this->Delete();
		SpawnCoin();
	}
}

void CBrick::OnNoCollision(DWORD dt)
{
	if (type != GOLD_BRICK) return;

	y += vy * dt;
}

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (type == STRIPE_BRICK)
		animations->Get(ID_ANI_BRICK)->Render(x, y, false);
	else if(state == GOLD_BRICK_STATE_IS_HITTED)
		animations->Get(ID_ANI_QUESTION_BRICK_HITTED)->Render(x, y, false);
	else 
		animations->Get(ID_ANI_GOLD_BRICK)->Render(x, y, false);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	switch (state)
	{
	case GOLD_BRICK_STATE_IS_HITTED:
		hit_start = GetTickCount64();

		break;
	}
	CGameObject::SetState(state);
}

void CBrick::SpawnSwitch()
{
	this->_switch = new CSwitch(this->x, this->y - BRICK_BBOX_HEIGHT / 2 - 8.5f);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(_switch);
}

void CBrick::SpawnCoin()
{
	CCoin* coin = new CCoin(x, y);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(coin);
}