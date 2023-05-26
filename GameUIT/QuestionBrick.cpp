#include "QuestionBrick.h"
#include "debug.h"

void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != QBRICK_STATE_HITTED)
		animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y);
	else 
		animations->Get(ID_ANI_QUESTION_BRICK_HITTED)->Render(x, y);
}

void CQuestionBrick::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y <= minY)
	{
		vy = BOUNCING_SPEED;
	}
	if (y > currentY)
	{
		y = currentY;
		vy = 0;
	}

	CGameObject::Update(dt,coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
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
		vy = -BOUNCING_SPEED;
		break;
	}

    CGameObject::SetState(state);
}