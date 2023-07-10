#include "Entrance.h"
#include "DataBinding.h"

void CEntrance::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int numEtrPass = CDataBindings::GetInstance()->NumEntrancePass;
	for (int i = 0; i < numEtrPass; i++)
	{
		if (this->type == CDataBindings::GetInstance()->WorldEntrance[i].ID
			&& CDataBindings::GetInstance()->WorldEntrance[i].isPassed)
			SetState(ENTRANCE_STATE_BEING_PASSED);
	}
}

void CEntrance::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != ENTRANCE_STATE_BEING_PASSED)
		animations->Get(ID_ANI_ENTRANCE_START + type)->Render(x, y, false);
	else 
		animations->Get(ID_ANI_ENTRANCE_MAP_M)->Render(x, y, false);
}

void CEntrance::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - ENTRANCE_BBOX_WIDTH / 2;
	top = y - ENTRANCE_BBOX_HEIGHT / 2;
	right = left + ENTRANCE_BBOX_WIDTH;
	bottom = top + ENTRANCE_BBOX_HEIGHT;
}

void CEntrance::SetState(int state)
{
	this->state = state;
}