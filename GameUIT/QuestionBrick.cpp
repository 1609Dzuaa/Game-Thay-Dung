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

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (isBouncing)
	{
		if (GetTickCount64() - bouncing_start > BOUNCING_TIME)
		{
			isBouncing = false;
			y += BOUNCING_DISTANCE;
		}
	}

	CGameObject::Update(dt);
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
		bouncing_start = GetTickCount64();
		isBouncing = true;
		y -= BOUNCING_DISTANCE;
		break;
	}

    CGameObject::SetState(state);
}