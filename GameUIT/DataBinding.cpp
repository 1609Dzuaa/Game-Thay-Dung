#include "DataBinding.h"
#include "Entrance.h"

CDataBindings* CDataBindings::__DBInstance = nullptr;

Entrance CDataBindings::WorldEntrance[7] = { -1,0 };
int CDataBindings::NumEntrancePass = 0;

CDataBindings* CDataBindings::GetInstance()
{
	if (!__DBInstance)
		__DBInstance = new CDataBindings();
	return __DBInstance;
}