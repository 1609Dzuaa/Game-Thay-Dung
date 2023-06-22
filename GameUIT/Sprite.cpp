#include "Sprite.h"
#include "Camera.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / texWidth;
	sprite.TexCoord.y = this->top / texHeight;

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / texWidth;
	sprite.TexSize.y = spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

void CSprite::Draw(float x, float y)
{
	CGame* g = CGame::GetInstance();
	D3DXVECTOR2 cam = CCamera::GetInstance()->GetCamPos(); //Lấy vị trí cam để vẽ
	D3DXMATRIX matTranslation;

	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	//Trừ đi 1 lượng cx để vẽ nó ở vị trí mới
	//Nếu không, map sẽ không di chuyển
	//Đoạn: (FLOAT)floor(cam.x) rất quan trọng, nếu 0 có nó thì khi di chuyển map sẽ trông như bị GLITCH
	D3DXMatrixTranslation(&matTranslation, x - (FLOAT)floor(cam.x), g->GetBackBufferHeight() - y + (FLOAT)floor(cam.y), 0.1f);

	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}