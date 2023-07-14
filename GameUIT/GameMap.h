#pragma once
#include <d3d10.h>
#include"Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "AssetIDs.h"
#include "Camera.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class CMap
{
	//Vì map là mảng 2 chiều nên cần lưu thông tin:
	//Số hàng, số cột của map cũng như số hàng và số cột của Tileset
	int MapRows, MapCollums, TilesetRows, TilesetCollums;
	int NumberofSprites; //Số lượng Sprites trong Tileset
	int start_draw_at_index_y;	//Dùng để nhận biết chỉ số y được vẽ bắt đầu từ đâu, mục đích giấu đi Underworld ;)
	LPTEXTURE Texture_TileSet; //nguồn ảnh Tileset
	vector<LPSPRITE> SpritesSplitted; //vector chứa từng Sprite được cắt ra từ nguồn ảnh Tileset
	int** Map_Matrix;  //Ma trận map là con trỏ cấp 2
	CCamera* Cam;
public:
	//Only render & update things in Camera
	CMap(int TileSetID, int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int  TotalColumnsOfTileSet, int TotalTiles, int dr_st_y);
	~CMap();
	void ClipSpritesFromTileset(); //cắt từng Sprites từ Tileset
	void Render();
	void SetMapMatrix(int** map_matrix);
	void isViewable(int& Start_col, int& End_col, int& Start_row, int& End_row);
};
