#pragma once
#include "Entrance.h"

//Class này dùng để liên kết dữ liệu giữa 2 màn chơi là 1-1 và WORLD
//Xem xét có nên tống Card vào đây ? :Đ

struct Entrance
{
	int ID;
	BOOLEAN isPassed;
	float x, y;
	//Chứa ID của cái Entrance đó 
	//Một biến kiểm tra xem đã Pass Entrance đó hay chưa
	//Và vị trí của Entrance đấy để set vị trí mới cho MarioWorld
};

class CDataBindings
{
	static CDataBindings* __DBInstance;	//Đảm bảo chỉ có duy nhất 1 Instance
public:
	static Entrance WorldEntrance[7]; //Mảng các Entrance đã vượt qua
	static int NumEntrancePass; //Số lượng Entrance đã vượt qua
	CDataBindings() {};
	~CDataBindings() {};
	static CDataBindings* GetInstance();
};