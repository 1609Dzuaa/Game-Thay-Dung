#include "Entrance.h"

void CEntrance::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	//int id = static_cast<int>(ID_ANI_ENTRANCE_START) + type;
	/*if (type == 0)
	{
		animations->Get(ID_ANI_ENTRANCE_START)->Render(x, y, false);
	}
	else if (type == 1)
	{
		animations->Get(ID_ANI_ENTRANCE_MAP_1)->Render(x, y, false);
	}
	else if (type == 2)
	{
		animations->Get(ID_ANI_ENTRANCE_MAP_2)->Render(x, y, false);
	}
	else if (type == 3)
	{
		animations->Get(ID_ANI_ENTRANCE_MAP_3)->Render(x, y, false);
	}
	else if (type == 4)
	{
		animations->Get(ID_ANI_ENTRANCE_MAP_4)->Render(x, y, false);
	}*/
	animations->Get(ID_ANI_ENTRANCE_START + type)->Render(x, y, false);
}