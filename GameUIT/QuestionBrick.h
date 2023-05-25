#include "Animation.h"
#include "Animations.h"
#include "Brick.h"


#define ID_ANI_QUESTION_BRICK 20000
#define ID_ANI_QUESTION_BRICK_HITTED 20100
#define QBRICK_WIDTH 16
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define QBRICK_STATE_IDLE 0
#define QBRICK_STATE_HITTED 10

class CQuestionBrick :public CBrick 
{
	BOOLEAN isDestroyed;
public:
	CQuestionBrick(float x, float y) : CBrick(x, y) { isDestroyed = false; }
	void Render();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	
	int IsCollidable()
	{
		return (state != QBRICK_STATE_HITTED);
	}

	void SetState(int state);
};