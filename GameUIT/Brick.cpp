#include "Game.h"
#include "Brick.h"
void CBrick::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(510);
	ani->Render(x, y);
}