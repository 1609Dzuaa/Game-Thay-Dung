#include "Switch.h"

void CSwitch::Update(DWORD dt)
{
	//hiệu ứng rung lắc khi bật công tắc
	//và biến mọi gạch vàng thành vàng trừ cái gạch chứa nó
	

}

void CSwitch::Render()
{
	if (state == SWITCH_STATE_NORMAL)
		CAnimations::GetInstance()->Get(ANI_SWITCH)->Render(x, y, true);
	else 
		CAnimations::GetInstance()->Get(ANI_SWITCH_HITTED)->Render(x, y, true);
}

void CSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SWITCH_BBOX_WIDTH / 2;
	t = y - SWITCH_BBOX_HEIGHT / 2;
	r = l + SWITCH_BBOX_WIDTH;
	b = t + SWITCH_BBOX_HEIGHT;
};

void CSwitch::SetState(int state)
{
	switch (state)
	{
	case SWITCH_STATE_HITTED:
		y += 6.0f;

		break;
	}

	CGameObject::SetState(state);
}