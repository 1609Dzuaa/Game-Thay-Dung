#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define Goomba_Die 503

class CGoomba : public CGameObject
{
private:
	float vx;
public:
	CGoomba(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};