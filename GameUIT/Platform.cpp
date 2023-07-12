#include "Platform.h"
#include "Flower.h"

#include "Sprite.h"
#include "Sprites.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Camera.h"

void CPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


}

void CPlatform::Render()
{
	//if (!CCamera::GetInstance()->isViewable(this)) return;

	if (this->spriteIdBegin == 0) return; //0 vẽ
	if (this->length <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);

	//
	RenderBoundingBox();
	//
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length - cellWidth_div_2;
	b = t + this->cellHeight;
}