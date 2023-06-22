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
	this->Cam = CCamera::GetInstance();
	Map_Matrix = NULL;
}

CMap::~CMap()
{
	//release
}

void CMap::isViewable(int& Start_col, int& End_col, int& Start_row, int& End_row)
{
	Start_col = static_cast<int>(Cam->GetCamPos().x / TILE_WIDTH);
	End_col = static_cast<int>((Cam->GetCamPos().x + SCREEN_WIDTH) / TILE_WIDTH);
	Start_row = static_cast<int>(Cam->GetCamPos().y / TILE_HEIGHT);
	End_row = static_cast<int>((Cam->GetCamPos().y + SCREEN_HEIGHT) / TILE_HEIGHT);
	//Hàm này để xác định các cột, các hàng Viewable
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
		//dunno why minus 1, if not map'll looks terrible asf!

		LPSPRITE sprite = new CSprite(TileNum, left, top, right, bottom, Texture_TileSet);
		this->SpritesSplitted.push_back(sprite);
	}
	//Hàm này để cắt từng sprites có trong Tileset
	//Và lưu nó vào vector 
	DebugOut(L"Clip Sprites Successfully!\n");
}

void CMap::Render()
{
	int Viewable_Col_start = 0;
	int Viewable_Col_end = 0;
	int startRow = 0;
	int endRow = 0;

	isViewable(Viewable_Col_start, Viewable_Col_end, startRow, endRow);

	for (int CurrentRow = startRow; CurrentRow < endRow; CurrentRow++)
		for (int CurrentColumn = Viewable_Col_start; CurrentColumn < Viewable_Col_end; CurrentColumn++)
		{
			int Sprite_ID = Map_Matrix[CurrentRow][CurrentColumn];
			//Chỉ số ID của Sprites trong Tileset sẽ tương ứng với chỉ số trong vector SpritesSplited
			float Draw_X = float(CurrentColumn * TILE_WIDTH);
			float Draw_Y = float(CurrentRow * TILE_HEIGHT);
			SpritesSplitted.at(Sprite_ID)->Draw(Draw_X, Draw_Y); //Vẽ Sprites được tách tại vị trí x, y
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
