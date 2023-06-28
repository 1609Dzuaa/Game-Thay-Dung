#include "Animation.h"
#include "Animations.h"
#include "Brick.h"
#include "Mushroom.h"
#include "PlayScene.h"

//Map 1-1 Question Brick cho ra 3 loại items: coin, nấm, lá
#pragma region QUESTION_BRICK_TYPE 

#define HAS_NO_ITEM 0
#define HAS_ITEM 1

#define HAS_COIN 2
#define HAS_MUSHROOM 3
#define HAS_LEAF 4

#pragma endregion 

#define ID_ANI_QUESTION_BRICK 20000
#define ID_ANI_QUESTION_BRICK_HITTED 20100
#define QBRICK_WIDTH 16
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define QBRICK_STATE_HITTED 10

#define BOUNCING_DISTANCE 5.0f
#define BOUNCING_SPEED 0.1f

class CQuestionBrick :public CGameObject 
{
	float minY, currentY;
	bool isEmpty; //check đã bóc hàng hay chưa
	int HasItem; //Có quà hay không
	int type;
	CMushroom* mr = NULL;
	BOOLEAN blockMushroom; //dùng để block kh cho nấm sinh ra khi hit gạch ở level 2 -> 1
	//thêm 1 biến block lá ?
public:

	CQuestionBrick(float x, float y, int Has_Item) : CGameObject(x, y)
	{
		CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		this->currentY = y;
		this->minY = y - QBRICK_BBOX_HEIGHT; //Càng lên cao y càng giảm
		this->HasItem = Has_Item;
		this->type = -1;
		if (!this->HasItem)
			this->type = HAS_COIN;
		else 
		{
			mr = new CMushroom(x, y, y - MUSHROOM_BBOX_HEIGHT + 0.2f, RED_MUSHROOM);
			current_scene->AddObjectToScene(mr);
			//Tạo sẵn nấm và thêm nó vào scene, mục đích để vẽ "đè" khi nấm đang chui lên
			//Careful: How 'bout mario level > small & this mr had never been used ?!
		}
		isEmpty = false;
		blockMushroom = false;
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