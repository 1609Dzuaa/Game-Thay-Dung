#pragma once
#include <d3d10.h>
#include"Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "AssetIDs.h"
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class CMap
{
	//Tham khảo ý tưởng từ: https://github.com/lequockhanh19521680/gamemario/blob/master/05-SceneManager/Map.h

	//Vì map là mảng 2 chiều nên cần lưu thông tin:
	//Số hàng, số cột của map cũng như số hàng và số cột của Tileset
	int MapRows, MapCollums, TilesetRows, TilesetCollums;
	int NumberofSprites; //Số lượng Sprites trong Tileset	
	int startX;
	int startY;
	LPTEXTURE Texture_TileSet; //nguồn ảnh Tileset
	vector<LPSPRITE> SpritesSplited; //vector chứa từng Sprite được cắt ra từ nguồn ảnh Tileset
	int** Map_Matrix;  //Ma trận map là con trỏ cấp 2
	float CamX, CamY;
public:
	CMap(int TileSetID, int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int  TotalColumnsOfTileSet, int TotalTiles, int startX, int startY);
	~CMap();
	void ClipSpritesFromTileset(); //cắt từng Sprites từ Tileset
	void Render();
	void SetTileMapData(int** TileMapData);
	int GetMapWidth();
	int GetMapHeight();
};
