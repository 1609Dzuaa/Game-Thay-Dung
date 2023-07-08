#include "BlockPoint.h"

void CBlockPoint::Render()
{
	//RenderBoundingBox();
}

void CBlockPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BLOCKPOINT_BBOX_WIDTH / 2;
	top = y - BLOCKPOINT_BBOX_HEIGHT / 2;
	right = left + BLOCKPOINT_BBOX_WIDTH;
	bottom = top + BLOCKPOINT_BBOX_HEIGHT;
}