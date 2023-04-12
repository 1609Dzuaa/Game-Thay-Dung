#pragma once
#include <unordered_map>
#include <d3dx10.h>

#include "Texture.h"

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures* __instance;

	unordered_map<int, LPTEXTURE> textures;
	//cấu trúc dữ liệu unordered_map với biến là textures dùng để lưu trữ 
	//các textures thông qua dạng Key - Value tương tự như Hash Table

public:
	CTextures();
	void Add(int id, LPCWSTR filePath);
	LPTEXTURE Get(unsigned int i); //trả về một Textures 

	static CTextures* GetInstance();
};