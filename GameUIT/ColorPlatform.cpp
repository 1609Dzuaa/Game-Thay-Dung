#include "ColorPlatform.h"
#include "Textures.h"
#include "Camera.h"
#include "debug.h"

void CColorPlatform::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	if (spriteIdBegin != 0)
	{
		s->Get(this->spriteIdBegin)->Draw(xx, y);
		xx += this->cellWidth;
		for (int i = 1; i < this->length - 1; i++)
		{
			s->Get(this->spriteIdMiddle)->Draw(xx, y);
			xx += this->cellWidth;
		}
		if (length > 1)
		{
			s->Get(this->spriteIdEnd)->Draw(xx, y);
		}
	}
	RenderBoundingBox();
	
}

void CColorPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length - cellWidth_div_2;
	b = t + this->cellHeight;
}

void CColorPlatform::RenderBoundingBox()
{
	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	cx = CCamera::GetInstance()->GetCamPos().x;
	cy = CCamera::GetInstance()->GetCamPos().y;

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx + 5, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right + 10, rect.bottom - 1);
}