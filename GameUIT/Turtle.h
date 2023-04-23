#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define Goomba_Die 503

class CTurtle : public CGameObject
{
private:
	float vx;
public:
	CTurtle(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};