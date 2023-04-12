#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define MARIO_WIDTH 14

class CMario : public CGameObject
{
protected:
	int ID;
	float vx;
public:
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};