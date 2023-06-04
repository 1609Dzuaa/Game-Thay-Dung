#include "ColorBox.h"

void CColorBox::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COLOR_BOX)->Render(x, y);
}