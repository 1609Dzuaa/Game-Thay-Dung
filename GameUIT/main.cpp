/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Animation.h"
#include "Animations.h"


#include "Mario.h"
#include "Brick.h"
#include "Enemy.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255,0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_BRICK 30

//#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO L"mario_transparent.png"
#define TEXTURE_PATH_MISC L"misc_transparent.png"
#define TEXTURE_PATH_ENEMIES L"enemies_transparent.png"
#define TEXTURE_PATH_BRICK L"brick.png"

CMario* mario;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_START_VX 0.1f
CEnemy* Enemy;

CBrick* brick[50];

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
*/

void LoadResources()
{
	//Tạo 1 con trỏ kiểu CTextures tên là textures để nắm giữ toàn bộ textures của game
	CTextures* textures = CTextures::GetInstance();

	//Thêm các đường dẫn file texture cùng với đó là ID của chúng để dễ phân biệt
	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMIES);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_BRICK, TEXTURE_PATH_BRICK);

	//Tạo 1 con trỏ kiểu CSprites để nắm giữ toàn bộ sprites của game
	CSprites* sprites = CSprites::GetInstance();

	//Tạo 1 con trỏ kiểu CTexture tên là texMario để lấy Textures của Mario từ biến textures đã lưu trước đó  
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// readline => id, left, top, right

    //Thêm Sprites của Mario lúc đi
	//id có thể là bất kì, tuy nhiên nên đặt sao cho dễ phân biệt, dễ nhớ
	sprites->Add(10001, 246, 154, 259, 181, texMario);
	sprites->Add(10002, 275, 154, 290, 181, texMario);
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	//Thêm Sprites của Mario lúc về
	sprites->Add(10011, 186, 154, 200, 181, texMario);
	sprites->Add(10012, 155, 154, 171, 181, texMario);
	sprites->Add(10013, 125, 154, 141, 181, texMario);

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	//Thêm Sprites của cục gạch bí ẩn
	sprites->Add(20001, 352, 153, 369, 168, texMisc);
	//sprites->Add(20002, 318, 117, 334, 133, texMisc);
	//sprites->Add(20003, 336, 117, 352, 133, texMisc);
	//sprites->Add(20004, 354, 117, 370, 133, texMisc);

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(31001, 4, 13, 22, 30, texEnemy);
	sprites->Add(31002, 24, 13, 42, 30, texEnemy);

	//Tạo biến con trỏ kiểu CAnimations để chứa toàn bộ animation của game
	CAnimations* animations = CAnimations::GetInstance();

	//Tạo biến con trỏ kiểu Animation để thêm các chuyển động của nhân vật
	LPANIMATION ani;

	ani = new CAnimation(200); //Thiết lập thời gian animation mặc định là 200ms
	ani->Add(10001); //cụp 
	ani->Add(10002); //giơ chân vừa
	ani->Add(10003); //giơ chân max

	//Thêm 3 chuyển động của Mario theo thứ tự lúc đi và thêm nó vào biến animations với ID là 500
	//và biến con trỏ lúc nãy đã lưu chuyển động của nhân vật
	animations->Add(500, ani);

	ani = new CAnimation(200);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);

	//Thêm chuyển động Mario lúc về vào biến animations với ID là 501 
	animations->Add(501, ani);

	ani = new CAnimation(200);
	ani->Add(20001, 1000);
	//ani->Add(20002);
	//ani->Add(20003);
	//ani->Add(20004);
	animations->Add(510, ani);
	//Chuyển động của cục gạch

	ani = new CAnimation(200);
	ani->Add(31001);
	ani->Add(31002);
	animations->Add(502, ani);

	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX);
	Enemy = new CEnemy(40.0f, 136.0f, 0.05f);
	for (int i = 0; i < 20; i++)
		brick[i] = new CBrick(0.0f+16*i, 150.0f);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	mario->Update(dt);
	Enemy->Update(dt);
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		for (int i = 0; i < 20; i++)
			brick[i]->Render();
		mario->Render();
		Enemy->Render();

		// Uncomment this line to see how to draw a porttion of a texture
		//LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);


		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}