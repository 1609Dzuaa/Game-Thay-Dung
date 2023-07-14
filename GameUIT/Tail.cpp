#include "Tail.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Flower.h"
#include "Mario.h"
#include "PlayScene.h"
#include "EffectCollision.h"
#include "Brick.h"
#include "Mario.h"

CTail::CTail(float Mario_posX, float Mario_posY, int MARIO_BBOX_WIDTH, int nx, float mario_vx) :CGhostObject(x, y)
{
	if (nx > 0)
	{
		if (mario_vx > 0) //đang di chuyển
		{
			this->x = Mario_posX + MARIO_BBOX_WIDTH / 2 - 3.0f;
			this->vx = TAIL_SPEED_X_MARIO_MOVE;
			DebugOut(L"Nx>0, vx, Vx: %f, %f\n", vx, mario_vx);
		}
		else
		{
			this->x = Mario_posX - MARIO_BBOX_WIDTH / 2 - 8.0f;
			this->vx = TAIL_SPEED_X_MARIO_IDLE;
			DebugOut(L"Nx>0, vx, Vx: %f, %f\n", vx, mario_vx);
		}
	}
	else
	{
		if (mario_vx < 0)
		{
			this->x = Mario_posX + MARIO_BBOX_WIDTH / 2 + 3.0f;
			this->vx = -TAIL_SPEED_X_MARIO_MOVE;
			DebugOut(L"Nx<0, vx, Vx: %f, %f\n", vx, mario_vx);
		}
		else
		{
			this->x = Mario_posX + MARIO_BBOX_WIDTH / 2 + 8.0f;
			this->vx = -TAIL_SPEED_X_MARIO_IDLE;
			DebugOut(L"Nx<0, vx, Vx: %f, %f\n", vx, mario_vx);
		}
	}
	this->y = Mario_posY + 5.0f;

};

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

/*void CTail::UpdateTailPos()
{
	this->x = obj_mario->GetX() + 14 / 2 + 8.0f;
	this->y = obj_mario->GetY() + 5.0f;
}*/

void CTail::Render()
{
	//RenderBoundingBox(); 
}

void CTail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	this->y = mario->GetY() + 5.0f;
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//kh tấn công thì bỏ qua hàm này
	if (!mario->GetIsAttacking()) return;
	
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	if (dynamic_cast<CFlower*>(e->obj))
		OnCollisionWithFlower(e); //bởi vì nó 0 có vận tốc và cây cũng thế nên 0 xảy ra va chạm
	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithGoldBrick(e);
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->SpawnScore(goomba);
	mario->SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
}

void CTail::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (koopa->GetType() == GREEN_FLYING_KOOPA)
	{
		koopa->SetType(GREEN_KOOPA);
		koopa->SetState(KOOPA_STATE_SLEEP_REVERSE);
		mario->SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
	}
	else
	{
		koopa->SetState(KOOPA_STATE_SLEEP_REVERSE);
		mario->SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
	}
}

void CTail::OnCollisionWithFlower(LPCOLLISIONEVENT e)
{
	CFlower* flower = dynamic_cast<CFlower*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	flower->SetState(FLOWER_STATE_DIE);
	mario->SpawnScore(e->obj);
	mario->SpawnEffect(e, this, EFF_COL_TYPE_NORMAL);
	//DebugOut(L"Tail Collided\n");
}

void CTail::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CBrick* br = dynamic_cast<CBrick*>(e->obj);
	if (br->GetType() == GOLD_BRICK)
	{
		//Nếu có item ở trong và chưa bị bóc
		if (br->GetItemType() != NO_ITEM && br->GetState() != GBRICK_HAS_ITEM_STATE_IS_HITTED)
		{
			br->SetSpeed(0, -GOLD_BRICK_BOUNCING_SPEED);
			br->SetHitted(true);
			br->SetState(GBRICK_HAS_ITEM_STATE_IS_HITTED);
		}
		else if(br->GetItemType() == NO_ITEM)
			br->SetState(GBRICK_STATE_IS_DESTROYED);
	}
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH / 2;
	t = y - TAIL_BBOX_HEIGHT / 2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}