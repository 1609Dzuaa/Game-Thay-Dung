#include "Brick.h"
#include "QuestionBrick.h"
#include "PlayScene.h"
#include "Switch.h"
#include "debug.h"

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	UpdatePosition(dt);
	UpdateGoldCoin();

	if (state == GOLD_BRICK_STATE_IS_HITTED && !IsDeleted() && _switch == NULL)
	{
		eff = new CEffectCollision(x, min_pos - 5.0f, hit_start, EFF_COL_TYPE_SMOKE_EVOLVE);
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		current_scene->AddObjectToScene(eff);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//Gold brick lúc vỡ ra cũng bị ảnh hưởng bởi StopWatch
	//DebugOutTitle(L"isBlock, state: %d, %d", IsBlocking(), state);
	//Gạch gold bị phá thì spawn mảnh nhỏ 4 hướng
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
		if (state == GOLD_BRICK_STATE_IS_HITTED && _switch == NULL)
			SpawnSwitch();
		y = old_pos;
		vy = 0;
	}
}

void CBrick::UpdateGoldCoin()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//ĐK trông khá là cồng kềnh, giải thích kẻo lại quên:
	//nếu Mario đã bấm công tắc và viên gạch vàng đó 0 có công tắc 
	//cũng như chưa bị biến thành vàng và trạng thái khác trạng thái bthg
	//thì mới biến nó thành vàng
	//Tại sao có đk cuối ? -> để tránh khi state là normal (hiển nhiên isTurnToCoin = false) dẫn đến
	//nó thoả đk và tiếp tục giữ state là turn to coin mãi
	if (mario->GetIsHitSwitch() && !this->hasSwitch && type == GOLD_BRICK && !isTurnToCoin && state != GOLD_BRICK_STATE_NORMAL)
		SetState(GOLD_BRICK_STATE_TURN_TO_COIN);	

	if (GetTickCount64() - gold_coin_start >= GOLD_COIN_TIME_OUT && isTurnToCoin)
		SetState(GOLD_BRICK_STATE_NORMAL);
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
		animations->Get(ID_ANI_STRIPE_BRICK)->Render(x, y, false);
	else if(state == GOLD_BRICK_STATE_IS_HITTED)
		animations->Get(ID_ANI_QUESTION_BRICK_HITTED)->Render(x, y, false);
	else if(state == GOLD_BRICK_STATE_TURN_TO_COIN)
		animations->Get(ID_ANI_GOLD_COIN)->Render(x, y, false);
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
	case GOLD_BRICK_STATE_TURN_TO_COIN:
		gold_coin_start = GetTickCount64();
		isTurnToCoin = true;

		break;

	case GOLD_BRICK_STATE_NORMAL:
		gold_coin_start = 0;
		isTurnToCoin = false;

		break;
	}
	CGameObject::SetState(state);
}

void CBrick::SpawnSwitch()
{
	this->_switch = new CSwitch(this->x, this->y - BRICK_BBOX_HEIGHT / 2 - 8.5f);
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->AddObjectToScene(_switch);
	hit_start = 0;
}