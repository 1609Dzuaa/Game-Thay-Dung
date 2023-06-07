﻿#include "GameMap.h"
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
		this->SpritesSplited.push_back(sprite);
	}
	//Hàm này để cắt từng sprites có trong Tileset
	//Và lưu nó vào vector 
	DebugOut(L"Clip Sprites Successfully!\n");
}

void CMap::Render()
{
	//Problem here, 1st & last = 0
	int FirstColumn = int(floor(CamX / TILE_WIDTH));
	int LastColumn = int(ceil((CamX * TILE_WIDTH + CGame::GetInstance()->GetScreenWidth()) / TILE_WIDTH));
	if (LastColumn >= MapCollums)
		LastColumn = MapCollums - 1;
	for (int CurrentRow = 0; CurrentRow < MapRows; CurrentRow++)
		for (int CurrentColumn = FirstColumn; CurrentColumn <= LastColumn; CurrentColumn++)
		{
			int index = Map_Matrix[CurrentRow][CurrentColumn];
			if (index < NumberofSprites)
			{
				float xDraw = float(CurrentColumn * TILE_WIDTH) + float(startX * TILE_WIDTH);
				float yDraw = float(CurrentRow * TILE_HEIGHT) - float(startY * TILE_HEIGHT);
				SpritesSplited.at(index)->Draw(xDraw, yDraw);
			}
		}

}

void CMap::SetTileMapData(int** map_mat)
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
