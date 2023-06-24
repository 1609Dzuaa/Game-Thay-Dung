#include "Animations.h"
#include "Switch.h"

void CSwitch::Update(DWORD dt)
{
	//hiệu ứng rung lắc khi bật công tắc
	//và biến mọi gạch vàng thành vàng
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
	l = x - 16 / 2;
	t = y - 16 / 2;
	r = l + 16;
	b = t + 16;
};