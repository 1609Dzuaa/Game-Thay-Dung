#include "Tail.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Flower.h"
#include "Mario.h"
#include "PlayScene.h"
#include "EffectCollision.h"
#include "Brick.h"
#include "Mario.h"

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//DebugOut(L"vx: %f\n", vx);
}

void CTail::Render()
{
	RenderBoundingBox(); 
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
	DebugOut(L"Tail Collided\n");
}

void CTail::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CBrick* br = dynamic_cast<CBrick*>(e->obj);
	/*if (br->GetType() == GOLD_BRICK)
	{
		if (br->GetHasSwitch())
		{
			br->SetState(GOLD_BRICK_HAS_SW_STATE_IS_HITTED);
		}
		else*/ //Hiện đang có lỗi phá gạch chứa Switch
			br->Delete();
		//Thêm hiệu ứng phá gạch
	//}
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH / 2;
	t = y - TAIL_BBOX_HEIGHT / 2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}