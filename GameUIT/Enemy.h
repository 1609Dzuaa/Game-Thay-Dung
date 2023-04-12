#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

class CEnemy : public CGameObject
{
private:
	float vx;
public:
	CEnemy(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};