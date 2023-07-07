#include "Sprite.h"
#include "Camera.h"
#include "PlayScene.h"
#include "Tube.h"
//Nên coi lại Render unrelated to Update (x, y,...) trong phần này
//Xong trans thì làm Card
//Ăn card thì tự động chạy đến cuối map

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
	this->shake_start = -1;
	this->shake_stop = 0;
	this->Shake = 0;
	this->noShake = 0;
	this->Initialized = 0;

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
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	/*if (mario->GetShaking() && !Initialized)
		StartShaking(); //khởi tạo và đánh dấu đã khởi tạo
	else if (mario->GetShaking())
		HandleShaking();

	if (Shake && mario->GetShaking())
	{
		x += 1.0f;
		y += 2.0f;
	}*/

	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	//Trừ đi 1 lượng cx để vẽ nó ở vị trí mới
	//Nếu không, map sẽ không di chuyển
	//Đoạn: (FLOAT)floor(cam.x) rất quan trọng, nếu 0 có nó thì khi di chuyển map sẽ trông như bị GLITCH
	D3DXMatrixTranslation(&matTranslation, x - (FLOAT)floor(cam.x), g->GetBackBufferHeight() - y + (FLOAT)floor(cam.y), 0.1f);
	this->sprite.matWorld = (this->matScaling * matTranslation);
	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

void CSprite::HandleShaking()
{
	//Shaking: vẽ trên 2 trục x, y tạo cảm giác như đang động đất
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (GetTickCount64() - shake_start >= SHAKE_TIME && Shake)
	{
		Shake = 0;
		shake_start = 0;
		noShake = 1;
		shake_stop = GetTickCount64();
	}
	else if (GetTickCount64() - shake_stop >= SHAKE_TIME && noShake)
	{
		Shake = 1;
		shake_start = GetTickCount64();
		noShake = 0;
		shake_stop = 0;
	}
}