#include "WinApp.h"


WinApp* WinApp::GetInstance() 
{
	static WinApp instance;
	return &instance;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}



void WinApp::Initialize(int windowWidth, int windowHeight, const std::string& windowTitle)
{
	
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
	
	wchar_t wtitle[256] = {};
	MultiByteToWideChar(CP_ACP, 0, windowTitle.c_str(), -1, wtitle, _countof(wtitle));

	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャを設定
	mWndClass.lpszClassName = wtitle;					// ウィンドウクラス名
	mWndClass.hInstance = GetModuleHandle(nullptr);		// ウィンドウハンドル
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定


	RegisterClassEx(&mWndClass); // ウィンドウクラスをOSに登録する
	RECT wrc = { 0, 0, mWindowWidth, mWindowHeight }; // ウィンドウサイズ{ X座標, Y座標, 横幅, 縦幅 }
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズを補正する

	// ウィンドウオブジェクトの生成
	mHwnd = CreateWindow(
		mWndClass.lpszClassName,		// クラス名
		wtitle,					// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			// 表示ｘ座標は OS にお任せ
		CW_USEDEFAULT,			// 表示ｙ座標は OS にお任せ
		wrc.right - wrc.left,	// ウィンドウ幅
		wrc.bottom - wrc.top,	// ウィンドウ高
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		mWndClass.hInstance,			// 呼び出しアプリケーションハンドル,
		nullptr);				// 追加パラメーター


	// ウィンドウを表示状態にする
	ShowWindow(mHwnd, SW_SHOW);
}

bool WinApp::WindowQUit() 
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// アプリケーションが終わるときに message が WM_QUIT になる
	return msg.message == WM_QUIT;
}

void WinApp::Finalize()
{
	UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}