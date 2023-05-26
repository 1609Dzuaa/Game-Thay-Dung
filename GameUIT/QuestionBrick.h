#include "Animation.h"
#include "Animations.h"
#include "Brick.h"


#define ID_ANI_QUESTION_BRICK 20000
#define ID_ANI_QUESTION_BRICK_HITTED 20100
#define QBRICK_WIDTH 16
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define QBRICK_STATE_HITTED 10

#define BOUNCING_TIME 500
#define BOUNCING_DISTANCE 5.0f
#define BOUNCING_SPEED 0.2f

class CQuestionBrick :public CBrick 
{
	float minY, currentY;
public:

	CQuestionBrick(float x, float y) : CBrick(x, y)
	{
		this->currentY = y;
		this->minY = y - QBRICK_BBOX_HEIGHT; //Càng lên cao y càng giảm
	}

	void Render();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SpawnCoin(float xBrick, float yBrick, float veloY);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);

	void OnNoCollision(DWORD dt);

	void HandleBouncingBrick();
};