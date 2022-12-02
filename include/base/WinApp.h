#pragma once
#ifndef WINAPI_H_
#define WINAPI_H_

#include <Windows.h>
#include <string>

#define MAIN WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)

class WinApp 
{

private: // メンバ変数

	WNDCLASSEX mWndClass = {};
	HWND mHwnd = nullptr;
	int mWindowWidth = 0;	// ウィンドウ幅
	int mWindowHeight = 0;	// ウィンドウ高

public: // 静的メンバ関数
	static WinApp* GetInstance();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	inline HWND GetHwnd() const { return mHwnd; }
	inline int GetWindowWidth() const { return mWindowWidth; }
	inline int GetWindowHeight() const { return mWindowHeight; }
	inline HINSTANCE GetHInstance() const { return mWndClass.hInstance; }

	void Initialize(int windowWidth = 1280, int windowHeight = 720, const std::string& windowTitle = "DirectXGame");
	bool WindowQUit();
	void Finalize();

private:
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

};


#endif // WINAPI_H_