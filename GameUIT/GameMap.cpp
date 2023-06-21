#include "GameMap.h"
#include "Utils.h"
#include "Game.h"
#include "debug.h"
CMap::CMap(int Tileset_Id, int Map_Rows, int Map_Collums, int Tileset_Rows, int  Tileset_Collums, int num_sprites, int startX, int startY)
{
	Texture_TileSet = CTextures::GetInstance()->Get(Tileset_Id);
	this->MapRows = Map_Rows;
	this->MapCollums = Map_Collums;
	this->TilesetRows = Tileset_Rows;
	this->TilesetCollums = Tileset_Collums;
	this->NumberofSprites = num_sprites;
	this->startX = startX;
	this->startY = startY;
	CamX = CamY = 0;
	Map_Matrix = NULL;
}

CMap::~CMap()
{
	//release
}

bool CMap::isInCamera(float x, float y)
{
	float w = 32.0f;
	float h = 32.0f;
	if (x + w <= (CGame::GetInstance()->GetCamX()) || (CGame::GetInstance()->GetCamX()) + SCREEN_WIDTH <= x)
		return false;
	if (y + h <= (CGame::GetInstance()->GetCamY()) || (CGame::GetInstance()->GetCamY()) + SCREEN_HEIGHT + h <= y)
		return false;
	return true;
}

void CMap::ClipSpritesFromTileset()
{
	DebugOut(L"Start Clipping Sprite\n");
	for (int TileNum = 0; TileNum < NumberofSprites; TileNum++)
	{
		int left = TileNum % TilesetCollums * TILE_WIDTH;
		int top = TileNum / TilesetCollums * TILE_HEIGHT;
		int right = left + TILE_WIDTH - 1;
		int bottom = top + TILE_HEIGHT - 1;
		//Công thức từ sách thầy

		LPSPRITE sprite = new CSprite(TileNum, left, top, right, bottom, Texture_TileSet);
		this->SpritesSplitted.push_back(sprite);
	}
	//Hàm này để cắt từng sprites có trong Tileset
	//Và lưu nó vào vector 
	DebugOut(L"Clip Sprites Successfully!\n");
}

void CMap::Render()
{
	int FirstColumn = int(floor(CamX / TILE_WIDTH));
	int LastColumn = int(ceil((CamX * TILE_WIDTH + CGame::GetInstance()->GetScreenWidth()) / TILE_WIDTH));
	if (LastColumn >= MapCollums)
		LastColumn = MapCollums - 1;
	for (int CurrentRow = 0; CurrentRow < MapRows; CurrentRow++)
		for (int CurrentColumn = FirstColumn; CurrentColumn <= LastColumn; CurrentColumn++)
		{
			int index = Map_Matrix[CurrentRow][CurrentColumn];
			//Chỉ số ID của Sprites trong Tileset sẽ tương ứng với chỉ số trong vector SpritesSplited
			if (index < NumberofSprites)
			{
				float Draw_X = float(CurrentColumn * TILE_WIDTH);
				float Draw_Y = float(CurrentRow * TILE_HEIGHT);
				SpritesSplitted.at(index)->Draw(Draw_X, Draw_Y);
				//Chỉ render những thứ trong Camera để tránh lãng phí tài nguyên
			}
		}

}

void CMap::SetMapMatrix(int** map_mat)
{
	Map_Matrix = map_mat;
}

int CMap::GetMapWidth()
{
	return MapCollums * TILE_WIDTH;
}

int CMap::GetMapHeight()
{
	return MapRows * TILE_HEIGHT;
}
