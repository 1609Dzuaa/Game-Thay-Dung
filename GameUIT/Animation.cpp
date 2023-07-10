#include "Animation.h"
#include "PlayScene.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	//Câu lệnh bên phải sẽ trả về một sprite, ta sẽ gán nó vào biến con trỏ sprite dưới
	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	//Tạo 1 con trỏ frame kiểu CAnimationFrame để khởi tạo giá trị cho nó
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);
	
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);

	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, BOOLEAN affectBySW)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (affectBySW)
	{
		ULONGLONG now = GetTickCount64(); //Hàm này trả về thời gian kể từ khi ứng dụng chạy cho đến hiện tại

		if (currentFrame == -1) //Nếu nó là frame mặc định (-1) //careful NULL here
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime(); //Lấy thời gian chuyển động của frame hiện tại và gán cho t
			if (now - lastFrameTime > t)
			{
				if (mario->GetStopWatch() && mario->GetIsAtWorld() || mario->HP < 0)
					; // nếu đang bật đồng hồ ngưng đọng thì giữ nguyên cái frame đó
				else
					currentFrame++;
				lastFrameTime = now;
				if (currentFrame == frames.size()) currentFrame = 0;
			}
		}
	}
	else
	{
		ULONGLONG now = GetTickCount64(); //Hàm này trả về thời gian kể từ khi ứng dụng chạy cho đến hiện tại

		if (currentFrame == -1) //Nếu nó là frame mặc định (-1) //careful NULL here
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime(); //Lấy thời gian chuyển động của frame hiện tại và gán cho t
			if (now - lastFrameTime > t)
			{
				currentFrame++;
				lastFrameTime = now;
				if (currentFrame == frames.size()) currentFrame = 0;
			}
		}
	}
	
	frames[currentFrame]->GetSprite()->Draw(x, y);
}