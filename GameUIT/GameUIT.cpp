/* =============================================================
INTRODUCTION TO GAME PROGRAMMING SE102

SAMPLE 00 - INTRODUCTORY CODE

This sample illustrates how to:

1/ Create a window
2/ Initiate Direct3D 10, DirectX Sprite
3/ Draw a static ball sprite to the screen
4/ Create frame rate independent movements

5/ Some good C++ programming practices
- Use constants whenever possible
- 0 Warnings

6/ Debug using __FILE__ __LINE__

HOW TO INSTALL Microsoft.DXSDK.D3DX
===================================
1) Tools > NuGet package manager > Package Manager Console
2) execute command :  Install-Package Microsoft.DXSDK.D3DX


WARNING: This one file example has a hell LOT of *sinful* programming practices
================================================================ */

/**/#include <windows.h>

#include <d3d10.h>
#include <d3dx10.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#include <comdef.h>

#define WINDOW_CLASS_NAME L"SampleWindow"
#define WINDOW_TITLE L"00 - Intro"
#define WINDOW_ICON_PATH L"ball.ico" 

HWND hWnd = 0;

// Each color is from 0.0f to 1.0f  ( 0/255 to 255/255 ) 
#define BACKGROUND_COLOR D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define MAX_FRAME_RATE 100

ID3D10Device* pD3DDevice = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D10RenderTargetView* pRenderTargetView = NULL;

//Đặt thằng này thành biến toàn cục để các hàm ở dưới có thể thoải mái truy cập
ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

int BackBufferWidth = 0;
int BackBufferHeight = 0;

#define TEXTURE_PATH_ball L"ball.png"
#define ball_START_X 8.0f
#define ball_START_Y 200.0f

#define ball_START_VX 0.2f
#define ball_START_VY 0.2f //new

#define ball_WIDTH 16.0f
#define ball_HEIGHT 16.0f


ID3D10Texture2D* texball = NULL;				// Texture object to store ball image
ID3DX10Sprite* spriteObject = NULL;				// Sprite handling object 

D3DX10_SPRITE spriteball;

float ball_x = ball_START_X;
float ball_vx = ball_START_VX;
float ball_y = ball_START_Y;
float ball_vy = ball_START_VY;

//Sử dụng Class để quản lý các đối tượng giống nhau
class Balls
{
//6 biến đại diện cho chiều dài, rộng, Toạ độ và tốc độ theo trục x và y của quả bóng
public: float height, width, x, y, vx, vy;
public: bool visible; //Dùng để Check xem vật thể có nằm trong vùng quan sát được ?!
	  //Sử dụng Hàm tạo để khởi tạo giá trị của từng quả bóng
	  Balls()
	  {
		  height = 16.0f;
		  width = 16.0f;
		  x = (float)(rand() % 600);
		  y = (float)(rand() % 450);
		  vx = 0.5f;
		  vy = 20.0f;
		  visible = TRUE;
	  }
};
Balls arrBalls[10]; //Mảng 10 đối tượng kiểu Quả Bóng
D3DX10_SPRITE spritePool[10]; //Mảng 10 Sprites của 10 Quả Bóng ở trên

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

// DEBUG SUPPORT FUNCTIONS //////////////
#define _W(x)  __W(x)
#define __W(x)  L##x

#define VA_PRINTS(s) {				\
		va_list argp;				\
		va_start(argp, fmt);		\
		vswprintf_s(s, fmt, argp);	\
		va_end(argp);				\
}		

void DebugOut(const wchar_t* fmt, ...)
{
	wchar_t s[4096];
	VA_PRINTS(s);
	OutputDebugString(s);
}

void DebugOutTitle(const wchar_t* fmt, ...)
{
	wchar_t s[1024];
	VA_PRINTS(s);
	SetWindowText(hWnd, s);
}
//////////////////////////////////////////

void InitDirectX(HWND hWnd)
{

	// retrieve client area width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	BackBufferWidth = r.right + 1;
	BackBufferHeight = r.bottom + 1;


	// Create & clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = BackBufferWidth;
	swapChainDesc.BufferDesc.Height = BackBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	HRESULT hr = S_OK;

	//
	// Uncomment the following section to query graphic cards on the computer
	//  

	/*
	IDXGIFactory* pFactory = NULL;
	#pragma comment(lib, "dxgi")
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (hr != S_OK)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();

		DebugOut((wchar_t*)L"[ERROR] CreateDXGIFactory has failed %s %d %d %s\n", _W(__FILE__), __LINE__, hr, errMsg);
		return;
	}

	IDXGIAdapter* pAdapter = NULL;
	for (UINT i = 0;
			pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
	 ++i)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		pAdapter->GetDesc(&adapterDesc);
		DebugOut((wchar_t*)L"[INFO] Adapter %d: %s\n",i, adapterDesc.Description);
	}
	*/

	// Create the D3D device and the swap chain
	hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3DDevice);

	if (hr != S_OK)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();

		DebugOut((wchar_t*)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d %d %s\n", _W(__FILE__), __LINE__, hr, errMsg);
		return;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// create the render target view
	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	// release the back buffer
	pBackBuffer->Release();

	// Make sure the render target view was created successfully
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// set the render target
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = BackBufferWidth;
	viewPort.Height = BackBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice->RSSetViewports(1, &viewPort);


	// create the sprite object to handle sprite drawing 
	hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);

	// Make sure the sprite creation was successful
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10CreateSprite has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	D3DXMATRIX matProjection;

	// Create the projection matrix using the values in the viewport
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width,
		(float)viewPort.TopLeftY,
		(float)viewPort.Height,
		0.1f,
		10);
	hr = spriteObject->SetProjectionTransform(&matProjection);

	DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");

	return;
}

/*
	Load game resources. In this example, we only load a ball image
*/

void LoadResources()
{
	ID3D10Resource* pD3D10Resource = NULL;

	// Loads the texture into a temporary ID3D10Resource object
	HRESULT hr = D3DX10CreateTextureFromFile(pD3DDevice,
		TEXTURE_PATH_ball,
		NULL,
		NULL,
		&pD3D10Resource,
		NULL);

	// Make sure the texture was loaded successfully
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] Failed to load texture file: %s \n", TEXTURE_PATH_ball);
		return;
	}

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&texball);
	pD3D10Resource->Release();

	if (!texball) {
		DebugOut((wchar_t*)L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return;
	}

	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	texball->GetDesc(&desc);

	// Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	// Clear out the shader resource view description structure
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));

	// Set the texture format
	SRVDesc.Format = desc.Format;
	// Set the type of resource
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	//ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

	pD3DDevice->CreateShaderResourceView(texball, &SRVDesc, &gSpriteTextureRV);

	for (int i = 0; i < 10; i++)
	{
		spritePool[i].pTexture = gSpriteTextureRV;
		spritePool[i].TextureIndex = 0;
		spritePool[i].TexCoord.x = 0;
		spritePool[i].TexCoord.y = 0;
		spritePool[i].TexSize.x = 1;
		spritePool[i].TexSize.y = 1;
		spritePool[i].ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	DebugOut((wchar_t*)L"[INFO] Texture loaded Ok: %s \n", TEXTURE_PATH_ball);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame

	IMPORTANT: no render-related code should be used inside this function.
*/

void Update(DWORD dt)
{    
	//Hàm này sẽ Update trạng thái từng object mình vẽ ra trong hàm Render()
	//Uncomment the whole function to see the ball moves and bounces back when hitting left and right edges
	float right_edge = BackBufferWidth;
	for (int i = 0; i < 10; i++)
	{
		if (arrBalls[i].visible)
		{
			arrBalls[i].x++;
			arrBalls[i].x += arrBalls[i].vx*dt;
			if (arrBalls[i].x <= 0 || arrBalls[i].x > right_edge)
			{
				arrBalls[i].vx = -arrBalls[i].vx;
				if (arrBalls[i].x < 0)
					arrBalls[i].x = 0;
				else if (arrBalls[i].x > right_edge)
					arrBalls[i].x = right_edge;
			}
		}
	}
}

/*
	Render a frame
	IMPORTANT: world status must NOT be changed during rendering
*/
void Render()
{
	if (pD3DDevice != NULL)
	{
		// clear the target buffer
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);
		if (spriteObject != NULL)
		{
			spriteObject->Begin(D3DX10_SPRITE_SORT_TEXTURE);
			D3DXMATRIX matTranslation;
			D3DXMATRIX matScaling;

			for (int i = 0; i < 10; i++)
			{
				if (arrBalls[i].visible)
				{
					D3DXMatrixScaling(&matScaling, arrBalls[i].width, arrBalls[i].height, 1.0f);
					D3DXMatrixTranslation(&matTranslation, arrBalls[i].x, (float)(BackBufferHeight - arrBalls[i].y), 0.1f);
					spritePool[i].matWorld = (matScaling * matTranslation);
				}
			}

			spriteObject->DrawSpritesBuffered(spritePool, 10); //Vẽ 10 Quả Bóng từ mảng các Sprites
			spriteObject->Flush();

			// Finish up and send the sprites to the hardware
			spriteObject->End();

			//DebugOutTitle((wchar_t*)L"%s (%0.1f,%0.1f) v:%0.1f", WINDOW_TITLE, ball_x, ball_y, ball_vx);

			// display the next item in the swap chain
		}
		pSwapChain->Present(0, 0);
	}
	//Hàm này chỉ để RENDERING
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	//Try this to see how the debug function prints out file and line 
	//wc.hInstance = (HINSTANCE)-100; 

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
			WINDOW_TITLE,
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
		DebugOut((wchar_t*)L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
			//FillArr();
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

void Cleanup()
{
	// release the rendertarget
	if (pRenderTargetView)
	{
		pRenderTargetView->Release();
	}
	// release the swapchain
	if (pSwapChain)
	{
		pSwapChain->Release();
	}
	// release the D3D Device
	if (pD3DDevice)
	{
		pD3DDevice->Release();
	}

	if (spriteObject)
	{
		spriteObject->Release();
		spriteObject = NULL;
	}

	DebugOut((wchar_t*)L"[INFO] Cleanup Ok\n");
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	hWnd = CreateGameWindow(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (hWnd == 0) return 0;

	InitDirectX(hWnd);

	LoadResources();
	Run();
	Cleanup();

	return 0;
}