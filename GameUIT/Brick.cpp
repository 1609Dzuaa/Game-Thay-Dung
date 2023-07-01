#include "Brick.h"
#include "QuestionBrick.h"
#include "PlayScene.h"
#include "Switch.h"
#include "BrickPiece.h"
#include "debug.h"

CBrick::CBrick(float x, float y, int type, int item_type) : CGameObject(x, y)
{
	this->type = type;
	this->item_type = item_type;
	if (this->item_type == MUSHROOM)
	{
		_mushroom = new CMushroom(x, y, y - BRICK_BBOX_HEIGHT, GREEN_MUSHROOM);
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		current_scene->AddObjectToScene(_mushroom);
	}
	else
		_mushroom = NULL;
	this->_switch = NULL;
	ay = GOLD_BRICK_GRAVITY;
	old_pos = this->y;
	min_pos = this->y - 10.0f;
	isBeingHitted = false;
	isTurnToCoin = false;
	hit_start = -1;
	gold_coin_start = -1;
	eff = NULL;
	_switch = NULL;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	UpdatePosition(dt);
	UpdateGoldCoin();

	//Chỉ GBrick chứa Switch thì mới cho khói

	CCollision::GetInstance()->Process(this, dt, coObjects);
	//Gold brick lúc vỡ ra cũng bị ảnh hưởng bởi StopWatch
	//Gạch gold bị phá thì spawn mảnh nhỏ 4 hướng
}

void CBrick::UpdatePosition(DWORD dt)
{
	if (y < min_pos) //prob here with  GBrick has Sw
	{
		vy = GOLD_BRICK_BOUNCING_SPEED;
	}
	if (y > old_pos)
	{
		//if it has switch, got hitted && at max pos => spawn switch
		
		if (state == GBRICK_HAS_ITEM_STATE_IS_HITTED && item_type == MUSHROOM)
			SpawnMushroom();
		else if (state == GBRICK_HAS_ITEM_STATE_IS_HITTED && item_type == SWITCH)
			SpawnSwitch(); //prob when spawn sw
		y = old_pos;
		vy = 0;
	}
}

void CBrick::UpdateGoldCoin()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//ĐK trông khá là cồng kềnh, giải thích kẻo lại quên:
	//nếu Mario đã bấm công tắc và viên gạch vàng đó 0 có Item 
	//cũng như chưa bị biến thành vàng và trạng thái khác trạng thái bthg
	//thì mới biến nó thành vàng
	//Tại sao có đk cuối ? -> để tránh khi state là normal (hiển nhiên isTurnToCoin = false) dẫn đến
	//nó thoả đk và tiếp tục giữ state là turn to coin mãi
	if (mario->GetIsHitSwitch() && item_type == NO_ITEM && type == GOLD_BRICK && !isTurnToCoin && state != GBRICK_STATE_NORMAL)
		SetState(GBRICK_STATE_TURN_TO_COIN);	

	if (GetTickCount64() - gold_coin_start >= GOLD_COIN_TIME_OUT && isTurnToCoin)
		SetState(GBRICK_STATE_NORMAL);
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
	else if(state == GBRICK_HAS_ITEM_STATE_IS_HITTED)
		animations->Get(ID_ANI_QUESTION_BRICK_HITTED)->Render(x, y, false);
	else if(state == GBRICK_STATE_TURN_TO_COIN)
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
	case GBRICK_HAS_ITEM_STATE_IS_HITTED:

		break;
	case GBRICK_STATE_TURN_TO_COIN:
		gold_coin_start = GetTickCount64();
		isTurnToCoin = true;

		break;
	case GBRICK_STATE_NORMAL:
		gold_coin_start = 0;
		isTurnToCoin = false;

		break;

	case GBRICK_STATE_IS_DESTROYED:
		SpawnBrickPiece();
		this->Delete();
		break;
	}

	CGameObject::SetState(state);
}

void CBrick::SpawnSwitch()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	this->_switch = new CSwitch(this->x, this->y - BRICK_BBOX_HEIGHT / 2 - 8.5f);
	current_scene->AddObjectToScene(_switch);

	hit_start = GetTickCount64(); //Dùng để đánh thgian spawn khói
	eff = new CEffectCollision(x, this->y - BRICK_BBOX_HEIGHT / 2 - 8.5f, hit_start, EFF_COL_TYPE_SMOKE_EVOLVE);
	current_scene->AddObjectToScene(eff);
	hit_start = 0;
}

void CBrick::SpawnMushroom()
{
	this->_mushroom->SetState(MUSHROOM_STATE_RISE_UP);
}

void CBrick::SpawnBrickPiece()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	//Spawn 4 mảnh vỡ: left top, left bot, right top, right bot
	CBrickPiece* piece1 = new CBrickPiece(x - 3.0f, y - 5.0f, -0.025f, 0.35f);
	CBrickPiece* piece2 = new CBrickPiece(x - 3.0f, y + 5.0f, -0.02f, 0.3f);
	CBrickPiece* piece3 = new CBrickPiece(x + 3.0f, y - 5.0f, 0.03f, 0.35f);
	CBrickPiece* piece4 = new CBrickPiece(x + 3.0f, y + 5.0f, 0.025f, 0.3f);
	current_scene->AddObjectToScene(piece1);
	current_scene->AddObjectToScene(piece2);
	current_scene->AddObjectToScene(piece3);
	current_scene->AddObjectToScene(piece4);
}