#include "Animation.h"
#include "Animations.h"
#include "Brick.h"

//Map 1-1 Question Brick cho ra 3 loại items: coin, nấm, lá
#pragma region QUESTION_BRICK_TYPE 

#define HAS_COIN 1
#define HAS_MUSHROOM 2
#define HAS_LEAF 3

#pragma endregion 

#define ID_ANI_QUESTION_BRICK 20000
#define ID_ANI_QUESTION_BRICK_HITTED 20100
#define QBRICK_WIDTH 16
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define QBRICK_STATE_HITTED 10


#define BOUNCING_DISTANCE 5.0f
#define BOUNCING_SPEED 0.1f

class CQuestionBrick :public CBrick 
{
	float minY, currentY;
	bool isEmpty;
	int type;
public:

	CQuestionBrick(float x, float y, int type) : CBrick(x, y)
	{
		this->currentY = y;
		this->minY = y - QBRICK_BBOX_HEIGHT; //Càng lên cao y càng giảm
		this->type = type;
		isEmpty = false;
	}

	void Render();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SpawnCoin(float xBrick, float yBrick, float veloY);

	void SpawnMushroom(float xBrick, float yBrick);

	void SpawnLeaf(float xBrick, float yBrick);

	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);

	void OnNoCollision(DWORD dt);

	void HandleBouncingBrick();
};