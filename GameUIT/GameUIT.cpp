#include<Windows.h>
#include <exception>
#pragma comment(lib, "user32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); 
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
		break;
	}
	//Hàm Xử Lý Thông Điệp Windows
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc; //Gán hàm xử lý cho con trỏ xử lý 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Tạo con trỏ chuột mũi tên truyền thống
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"GameWindow"; //Chữ "L" đằng trước tên chuỗi là cách để biên dịch chuỗi Unicode trong chương trình C/C++.
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);
	//Các dòng trên khởi tạo một biến kiểu WNDCLASSEXTENDED, thiết lập các giá trị và đăng ký cửa sổ biến đó

	HWND hWnd =
		    CreateWindow(
			L"GameWindow", // Tên lớp cửa sổ - Phải trùng với tên lớp đã đăng ký ở trên
			L"Super Mario Bros. 3",  
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,	
			CW_USEDEFAULT,
			800, //Rộng				
			600, //Dài
			NULL,
			NULL,
			hInst,
			NULL);
	//Gán hàm Tạo 1 cửa sổ cho biến kiểu HWND - Kiểu dữ liệu đại diện cho 1 cửa sổ

	if (!hWnd)
		return -1;
	ShowWindow(hWnd, nCmdShow); 
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//Vòng lặp trên nhận thông điệp từ người dùng - RẤT QUAN TRỌNG, nếu không có thì sẽ bị đơ. 
	//Với Game thì sẽ thay thế GetMess = PeekMess để trò chơi luôn chạy. 
	return 1;
}