#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

class CBrick : public CGameObject 
{
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};