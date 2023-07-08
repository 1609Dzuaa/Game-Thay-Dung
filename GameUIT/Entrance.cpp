#include "Entrance.h"

void CEntrance::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_ENTRANCE_START + type)->Render(x, y, false);
}

void CEntrance::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - ENTRANCE_BBOX_WIDTH / 2;
	top = y - ENTRANCE_BBOX_HEIGHT / 2;
	right = left + ENTRANCE_BBOX_WIDTH;
	bottom = top + ENTRANCE_BBOX_HEIGHT;
}